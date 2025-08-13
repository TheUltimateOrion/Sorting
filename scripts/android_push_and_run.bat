:: Usage: scripts/push_and_run_android.sh out/android-arm64/bin/hello_android [resource_dir] [libs_dir]
:: Why: /data/local/tmp is exec-friendly via adb; running from that directory preserves relative resource paths.

set REMOTE_DIR="/data/local/tmp/app/"

adb shell "rm -rf '%REMOTE_DIR%' && mkdir -p '%REMOTE_DIR%'"
adb push dist/OrionSort dist/lib/ dist/assets/ /data/local/tmp/app/

adb shell "cd '%REMOTE_DIR%' && chmod +x 'OrionSort' && export LD_LIBRARY_PATH=\$PWD/lib:\$PWD:\$LD_LIBRARY_PATH && './OrionSort'"
