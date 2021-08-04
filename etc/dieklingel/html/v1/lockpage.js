var passcode = "";

function addDigit(sender) {
    var digit = $(sender).text();
    passcode += digit;
    setDigit(passcode.length);
    if(passcode.length === 6) {
        console.log("passcode");

        var json = {
            body:{
                context: Context.EnterPasscode,
                data:{
                    passcode: passcode
                }
            }
        }
        core.Send(JSON.stringify(json));
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
    $('.passcodefield button').each((index, value) => {
        current[index] = $(value).text();
    });
    for(var i = 0; i < current.length; i++) {
        var tmp = current[i];
        var x = Math.floor(Math.random() * current.length);
        current[i] = current[x];
        current[x] = tmp;
    }
    $('.passcodefield button').each((index, value) => {
        $(value).prop('Counter', $(value).text()).animate({
            Counter: current[index]
        }, {
            duration: 300,
            step: (now) => {
                $(value).text(Math.ceil(now));
            }
        });
    });

    //$('.passcodefield button').each((index, value) => {
        /*var tmp = $(value).text(); 
        var x = Math.floor(Math.random() * $('.passcodefield button').length);
        //$(value).text($($('.passcodefield button')[x]).text()); 
        $(value).prop('Counter', $(value).text()).animate({
            Counter: $($('.passcodefield button')[x]).text()
        }, {
            duration: 1000,
            step: (now) => {
                $(value).text(Math.ceil(now));
            }
        });
        $($('.passcodefield button')[x]).text(tmp);

    }); */
}
