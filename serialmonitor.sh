if [ $# -eq 0 ]
  then
    echo "usage: serialmonitor.sh <baud>"
    exit 1
fi


stty $1 -F /dev/ttyACM0 raw -echo
cat /dev/ttyACM0
