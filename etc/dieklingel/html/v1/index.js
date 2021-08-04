var signs = ["sign1.html"]
var timeout = 5000;
var screenTimeout = setTimeout(showScreensaver, timeout);
var core = new dieklingel();
var hiddenButton = {
    timeout: null,
    detail: 0
};

window.onload = () => {
    // set the right height of all iframes
    var frames = document.getElementsByTagName("iframe");
    for(var frame of frames) {
        frame.style.height = frame.contentWindow.document.body.offsetHeight + 'px';
    }
    // detect contextmenue 
    $(this).on('contextmenu', (event) => {event.preventDefault()});
    // detect mousemove 
    //$(this).on('mousemove',  refreshScreentimeout);
    $('*').on('touchstart', refreshScreentimeout); 
    //$(this).on('scroll', refreshScreentimeout); 
    // create signs
    for(var i = 0; i < signs.length; i++) {
        var iframe = document.createElement('iframe');
        iframe.onload = (event) => {
            event.currentTarget.style.height = event.currentTarget.contentWindow.document.body.offsetHeight + 'px';
        }
        iframe.src = signs[i];
        document.getElementById('mainpage').appendChild(iframe);
    } 

    // create hidden button
    $('#hiddenButton').on('click', (event) => {
        clearTimeout(hiddenButton.timeout);
        hiddenButton.detail++;
        hiddenButton.timeout = setTimeout(() => {
            if(hiddenButton.detail === 3) {
                // normal zero button is clicked 3 times
                //window.open("http://localhost:8081/v3/secret.html", "_blank");
                window.open("http://localhost:8081/engine.html?page=v3/secret.html&r=35&t=20", "_blank");
            }
            hiddenButton.detail = 0;
        }, 200)
    });
    // create core
    
    $(core).on(Context.Movement, (event, data) => {
        // movement was detected
        console.log("movement was detected");
        hideScreensaver();
    });
    $(core).on(Context.ConnectionState, (event, data) => {
        if(data.body.data.state == "open") {
            // CT Successfully connected to CTS
            console.log("hide warnsign");
        }else {
            // CT is not connected to CTS
            console.log("show warnsign");
        }
    });
    $(core).on(Context.Unlock, (event, data) => {
        // unlock event is triggered
        console.log("show unlock animation");
        unlock();
    });
    //core.CreateEndpoint('ws://ubuntu-vm:8090/');
    core.CreateEndpoint('ws://dieklingel:8090/');
    hideScreensaver();
    randomizeDigits();
}

// Screensaver

function showScreensaver() {
    $('#mainpage')[0].scrollIntoView();
    $('.screensaver').css('display', 'block');
    removeDigit(6);
    randomizeDigits();
    $('.homescreen').css('display', 'none')
    var json = {
        body:{
            context:Context.DisplayState,
            data: {
                state:'off'
            }
        }
    }
    core.Send(JSON.stringify(json));
}

function hideScreensaver() {
    $('.screensaver').css('display', 'none');
    $('.homescreen').css('display', 'block')
    clearTimeout(screenTimeout);
    screenTimeout = setTimeout(showScreensaver, timeout);
    var json = {
        body:{
            context:Context.DisplayState,
            data: {
                state:'on'
            }
        }
    }
    core.Send(JSON.stringify(json));
}

function refreshScreentimeout() {
    console.log('scrrenTimeout');
    clearTimeout(screenTimeout);
    screenTimeout = setTimeout(showScreensaver, timeout);
}

// symbola
// unlock

var lock_animation_running = false;
function unlock() {
    if(!lock_animation_running ) {
        lock_animation_running = true;
        $('#lock').css("display", "block");
        $('#lock').fadeTo(400, 1, () => {
            $('#lock_top').animate({
                top: "180px"
            },{
                duration: 200,
                complete: () => { 
                    $('#lock').fadeTo(500, 0.1, () => {
                        $('#lock').css("display", "none");
                        $('#lock_top').css("top", "280px");
                        lock_animation_running = false;
                    });
                }
            });
        });
    } 
}

// warnsign
