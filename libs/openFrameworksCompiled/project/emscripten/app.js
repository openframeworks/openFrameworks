
document.addEventListener('DOMContentLoaded', () => {
  console.log('DOM fully loaded and parsed');
  const canvas = document.getElementById('canvas');
  if (!canvas) {
    console.error("Canvas element not found!");
    return;
  }
  canvas.addEventListener('contextmenu', (event) => {
      event.preventDefault();
  });

  function goEmscriptenFullscreen(resize) {
    Module.requestFullscreen(0, resize);
  }

  function tryFullScreen(aspect, resize) {
    var canvas = document.getElementById("canvas");
    if (resize) {
      canvas.width = screen.width;
      canvas.height = screen.height;
    }
    if (canvas.requestFullScreen) {
      if (aspect) goEmscriptenFullscreen(resize);
      else canvas.requestFullScreen();
    } else if (canvas.webkitRequestFullScreen) {
      if (aspect) goEmscriptenFullscreen(resize);
      else canvas.webkitRequestFullScreen();
    } else if (canvas.mozRequestFullScreen) {
      if (aspect) goEmscriptenFullscreen(resize);
      else canvas.mozRequestFullScreen();
    } else {
      canvas.width = window.innerWidth;
      canvas.height = window.innerHeight;
      document.getElementById("header").style.display = "none";
      document.getElementById("output").style.display = "none";
    }
  }
});

  var statusElement = document.getElementById("status");
  var progressElement = document.getElementById("progress");
  var spinnerElement = document.getElementById("spinner");

  var Module = {
    print: (function () {
      var element = document.getElementById("output");
      if (element) element.value = ""; // clear browser cache
      return (...args) => {
        var text = args.join(" ");
        console.log(text);
        if (element) {
          element.value += text + "\n";
          element.scrollTop = element.scrollHeight; // focus on bottom
        }
      };
    })(),
    canvas: (() => {
      var canvas = document.getElementById("canvas");
      canvas.addEventListener(
        "webglcontextlost",
        (e) => {
          alert("WebGL context lost. You will need to reload the page.");
          e.preventDefault();
        },
        false
      );
      return canvas;
    })(),
    setStatus: (text) => {
      Module.setStatus.last ??= { time: Date.now(), text: "" };
      if (text === Module.setStatus.last.text) return;
      var m = text.match(/([^(]+)\((\d+(\.\d+)?)\/(\d+)\)/);
      var now = Date.now();
      if (m && now - Module.setStatus.last.time < 30) return; // if this is a progress update, skip it if too soon
      Module.setStatus.last.time = now;
      Module.setStatus.last.text = text;
      if (m) {
        text = m[1];
        progressElement.value = parseInt(m[2]) * 100;
        progressElement.max = parseInt(m[4]) * 100;
        progressElement.hidden = false;
        spinnerElement.hidden = false;
      } else {
        progressElement.value = null;
        progressElement.max = null;
        progressElement.hidden = true;
        if (!text) spinnerElement.style.display = "none";
      }
      statusElement.innerHTML = text;
    },
    totalDependencies: 0,
    monitorRunDependencies: (left) => {
      this.totalDependencies = Math.max(this.totalDependencies, left);
      Module.setStatus(
        left
          ? `Preparing... (${this.totalDependencies - left}/${this.totalDependencies})`
          : "All downloads complete."
      );
    },
  };

  Module.setStatus("Downloading...");
  window.onerror = (text) => {
    console.log("onerror text:", text);
    const total = Module.totalDependencies || 1; // Default to avoid division by zero
    const left = text.match(/(\d+)\/(\d+)/);
    if (!left) {
      console.warn("Progress information missing or invalid.");
      Module.setStatus("Error: Progress information missing or invalid, see JavaScript console");
      return;
    }
    Module.setStatus("Exception thrown, see JavaScript console");
    spinnerElement.style.display = "none";
    Module.setStatus = (text) => {
      if (text) console.error(`[post-exception status] ${text}`);
    };
  };

  const fullscreenButton = document.getElementById('fullscreenButton');

  // Attach the event listener to handle fullscreen functionality
  fullscreenButton.addEventListener('click', () => {
    const pointerLock = document.getElementById('pointerLock').checked;
    const resize = document.getElementById('resize').checked;

    // Ensure `Module.requestFullscreen` is available before calling it
    if (typeof Module.requestFullscreen === 'function') {
      Module.requestFullscreen(pointerLock, resize);
    } else {
      console.error('Module.requestFullscreen is not defined.');
    }
  });
// });



