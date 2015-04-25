#!/bin/sh
gnome-terminal --title="Server $1" -x  sh -c "./server $1 serverPorts 2>/dev/null && gnome-terminal -x sh" &
gnome-terminal --title="Server $2" -x sh -c "./server $2 serverPorts 2>/dev/null && gnome-terminal -x sh" &
gnome-terminal --title="Server $3" -x sh -c "./server $3 serverPorts 2>/dev/null && gnome-terminal -x sh" &
