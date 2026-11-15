#!/vendor/bin/sh
# The AOSP Bluetooth HAL refuses to start without an address. The factory address lives in
# the MTK NVRAM file on /nvdata; publish it through the property the HAL reads.
f=/nvdata/APCFG/APRDEB/BT_Addr
[ -f "$f" ] || exit 0
addr=$(od -A n -t x1 -N 6 "$f" | tr -d '\n' | sed 's/^ *//;s/ *$//;s/ /:/g' | tr 'a-f' 'A-F')
case "$addr" in
    00:00:00:00:00:00|"") exit 0 ;;
esac
setprop persist.service.bdroid.bdaddr "$addr"
