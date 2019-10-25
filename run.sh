#! /bin/bash
for i in 1 2 3
do
    echo "org"
    time /home/panjie/web/src/v8/v8/out.gn/x64.release/d8 decode.js
    echo "opt"
    time /home/panjie/web/src/v8/v8/out.gn/x64.release/d8  --experimental-wasm-simd decode.js -- simd
done

