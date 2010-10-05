# hornetseye-xorg - Graphical output under X.Org
# Copyright (C) 2010 Jan Wedekind
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

# Namespace of Hornetseye computer vision library
module Hornetseye

  class X11Display

    class << self

      alias_method :orig_new, :new

      def new( name = ENV[ 'DISPLAY' ] )
        if name.nil? and ENV[ 'DISPLAY' ].nil?
          raise "DISPLAY variable was not set"
        end
        orig_new name
      end

      def show( *args, &action )
        options = args.last.is_a?( Hash ) ? args.pop : {}
        options = { :title => 'Hornetseye' }.merge options
        unless action
          options = { :output => XImageOutput }.merge options
          frame, width, height = *args
          width  ||= frame.width
          height ||= ( width.to_f * frame.height / frame.width ).round
          display = new
          output = options[ :output ].new
          output.write frame
          window = X11Window.new display, output, width, height
          window.title = options[ :title ]
          begin
            window.show
            display.event_loop
          ensure
            window.close
          end
        else
          options = { :output => XVideoOutput }.merge options
          width, height = *args
          result = action.call
          width  ||= result.shape[0]
          height ||= ( width.to_f * result.shape[1] / result.shape[0] ).round
          display = new
          output = options[ :output ].new
          window = X11Window.new display, output, width, height
          window.title = options[ :title ]
          begin
            window.show
            t = Time.new.to_f
            while true
              t += 1.0 / options[ :frame_rate ] if options[ :frame_rate ]
              output.write result
              delay = t - Time.new.to_f
              if delay > 0
                display.event_loop delay
              else
                display.process_events
              end
              break unless display.status?
              result = action.call
            end
          ensure
            window.close
          end
        end
      end

    end

    alias_method :orig_event_loop, :event_loop

    def event_loop( timeout = INFINITE )
      timeout *= 1000 unless timeout == INFINITE
      orig_event_loop timeout
    end

  end

end

