
/*
var wasmBinaryFile = 'jpgsquash.wasm';
const buffer = readbuffer(wasmBinaryFile);
const { instance } = await WebAssembly.instantiate(buffer);
*/


if (arguments.length == 0) {
    load("./pngsquash.js");
    console.log("scalar");
} else {
    load("./pngsquash-simd.js");
    console.log("simd");
}

Module['onRuntimeInitialized'] = onRuntimeInitialized;

function transcode(filename, quality)
{
    var imgAsArray = new Uint8Array(readbuffer(filename));
    var len = imgAsArray.byteLength;
    var buf = Module._malloc(len);
    //Module.HEAPU8.set(new Uint8Array(imgAsArray), buf);
    Module.HEAPU8.set(imgAsArray, buf);
    var size = Module._png_transcode(buf, len, quality);
    var result = new Uint8Array(Module.HEAPU8.buffer, buf, len);
    //console.log("org size = " + len + ", new file size = " + size);

}

function decode(filename, quality)
{
    var imgAsArray = new Uint8Array(readbuffer(filename));
    var len = imgAsArray.byteLength;
    var buf = Module._malloc(len);
    //Module.HEAPU8.set(new Uint8Array(imgAsArray), buf);
    Module.HEAPU8.set(imgAsArray, buf);
    var size = Module._png_decode(buf, len, quality);
    //console.log("org size = " + len + ", new file size = " + size);
    //console.log("size = " + size);

}



function onRuntimeInitialized() {

    var filelist = [
        //"666.png", "alphatest.png",
        //"Sample-png-image-200kb.png", "Sample-png-image-500kb.png",
        "Sample-png-image-1mb.png","SamplePNGImage_3mbmb.png", "SamplePNGImage_5mbmb.png"
        ];

    var quality = 100;
    //var dirname="images/"
    var dirname="images-up/images/"
    

    for (var i = 0; i < filelist.length; i++) {
        filename = dirname + filelist[i];

        for (var j = 0; j < 100; j++)
        {
            //transcode(filename, quality);
            decode(filename, quality);
        }
    }
}

