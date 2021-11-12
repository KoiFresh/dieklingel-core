import * as dup from "/viewport/dup.js"

var core = new dup.core("ws://ubuntu-vm:8080");

$(document).ready(() => {
    randomizeDigits();    
    $(".numpad button").on("click", (event) => {
        let btn = event.currentTarget;
        addDigit(btn);
    });

    $(".numpad #remove").on("click", (event) => {
        removeDigit(1);
    });

    $.get("/ressources/config.json", (config) => {
        $("#custom").html(config.custom.join(" "));
        $("#custom .event").on("click", (event) => {
            try {
                let str = $(event.currentTarget).attr("data");
                JSON.parse(str);
                core.send(str);
            }catch(e) {
                console.log("Error in custom button", e.message);
            }
        });
    });
});

var passcode = "";
function addDigit(sender) {
    var digit = $(sender).text();
    passcode += digit;
    setDigit(passcode.length);
    if(passcode.length === 6) {
        var json = {
            Body:{
                Context: "enter-passcode",
                Data:{
                    Passcode: passcode
                }
            }
        }
        core.send(JSON.stringify(json));
        passcode = "";
        setTimeout(() => {
            setDigit(passcode.length);
            randomizeDigits();  
        }, 300);
    } 
}

function removeDigit(length = 1) {
    passcode = passcode.substring(0, passcode.length - length);
    setDigit(passcode.length);
}

function setDigit(length) {
    var digits = $('.digit');
    for(var i = 0; i < digits.length; i++) {
        if(i > length - 1) {
            $(digits[i]).css('background-color', 'transparent');
        } else {
            $(digits[i]).css('background-color', 'white');
        }
    }
}

function randomizeDigits() {
    //console.log($('.passcodefield button')[0].innerText);
    var current = new Array();
    $('.numpad button').each((index, value) => {
        current[index] = $(value).text();
    });
    for(var i = 0; i < current.length; i++) {
        var tmp = current[i];
        var x = Math.floor(Math.random() * current.length);
        current[i] = current[x];
        current[x] = tmp;
    }
    $('.numpad button').each((index, value) => {
        $(value).prop('Counter', $(value).text()).animate({
            Counter: current[index]
        }, {
            duration: 300,
            step: (now) => {
                $(value).text(Math.ceil(now));
            }
        });
    });
}
