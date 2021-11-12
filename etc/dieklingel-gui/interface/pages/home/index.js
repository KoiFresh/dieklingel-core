$(document).ready(() => {
    $.get("/ressources/config.json", (config) => {
        for(let i = 0; i < config.signs.length; i++) {
            let frame = document.createElement("iframe", {is : "x-page"});
            frame.is = "x-page";
            frame.onload = (event) => {
                //event.currentTarget.style.height = event.currentTarget.contentWindow.document.body.offsetHeight + 'px';
                /*$(event.currentTarget.contentWindow.document).on('touchstart', () => {
                    //$("#screensaver").get(0).hide();
                    console.log("home/index.js touch");
                });*/
                $(event.currentTarget).attr("is", "x-page");
                if(typeof config.signs[i] === "object") {
                    frame.contentWindow.postMessage(config.signs[i].data, "*");
                }
            };
            if(typeof config.signs[i] === "string") {
                frame.src = "/ressources/" + config.signs[i];
            }else {
                frame.src = "/ressources/" + config.signs[i].name;
            }
            $(frame).addClass("sign");
            $("body").append(frame);
        } 
    });
});