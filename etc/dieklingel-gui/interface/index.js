import * as dup from "/viewport/dup.js";

var position = {
    top: 0,
    left: 0,
    x: 0,
    y: 0
}

$(document).ready(() => {
    $("#screensaver").on("click", (event) => {
        $(event.currentTarget).get(0).hide();
    });

    // i dont know why but jQuery is not working here
    window.top.addEventListener("message", (event) => {
        switch(event.data.event) {
            case "touchstart":
                $("#screensaver").get(0).hide();
                break;
        }
    });

    $.get("/ressources/config.json", (config) => {
        $("#screensaver").attr("timeout", config.timeout);
        $("#screensaver").attr("content", "/ressources/" + config.screensaver);
        $("#screensaver").get(0).draw();
    });
    
    var core = new dup.core("ws://ubuntu-vm:8080");
    $(core).on(dup.Context.Unlock, () => {
        console.log("unlock");
        unlock();
    });

    $(core).on(dup.Context.Movement, () => {
        console.log("movement");
        $("#screensaver").get(0).hide();
    });
});


const mouseDownHandler = function (e) {
    position = {
        left: $("#viewport").scrollLeft(),
        top: $("#viewport").scrollTop(),
        x: e.clientX,
        y: e.clientY,
    };
    document.addEventListener('mousemove', mouseMoveHandler);
    document.addEventListener('mouseup', mouseUpHandler);
}

const mouseMoveHandler = function (e) {
    // How far the mouse has been moved
    const dx = e.clientX - position.x;
    const dy = e.clientY - position.y;
    // Scroll the element
    $("#viewport").scrollTop(position.top - dy);
    $("#viewport").scrollLeft(position.left - dx);
};

const mouseUpHandler = function () {
    document.removeEventListener('mousemove', mouseMoveHandler);
    document.removeEventListener('mouseup', mouseUpHandler);
};

var lock_animation_running = false;
const unlock = function() {
    if(!lock_animation_running ) {
        lock_animation_running = true;
        $('#unlock').css("display", "block");
        $('#unlock').fadeTo(400, 1, () => {
            $('#unlock_top').animate({
                top: "180px"
            },{
                duration: 200,
                complete: () => { 
                    $('#unlock').fadeTo(500, 0.1, () => {
                        $('#unlock').css("display", "none");
                        $('#unlock_top').css("top", "280px");
                        lock_animation_running = false; 
                    });
                }
            }); 
        });
    }  
}