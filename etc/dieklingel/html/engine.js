var engine = {};

window.onload = () => {
    engine.url = getUrlParameter("page") ? getUrlParameter("page") : "v1/index.html";
    engine.left = getUrlParameter("l") ? getUrlParameter("l") : 5;
    engine.right = getUrlParameter("r") ? getUrlParameter("r") : 5;
    engine.top = getUrlParameter("t") ? getUrlParameter("t") : 5;
    engine.bottom = getUrlParameter("b") ? getUrlParameter("b") : 5; 
    engine.width = $(window).width() - engine.left - engine.right;
    engine.height = $(window).height() - engine.bottom - engine.top;
    
    console.log(engine);

     // $('#viewport').attr("src", "v1/x.html");
    engine.open(engine.url);
    if(getUrlParameter("debug")) {
        alert("width: " + engine.width + "; height: " + engine.height);
    }
}

function getUrlParameter(sParam) {
    var sPageURL = decodeURIComponent(window.location.search.substring(1)),
        sURLVariables = sPageURL.split('&'),
        sParameterName;

    var result = false;

    for(var i = 0; i < sURLVariables.length; i++) {
        sParameterName = sURLVariables[i].split('=');
        if (sParameterName[0] === sParam) {
            result = sParameterName[1] === undefined ? true : sParameterName[1];
        }
    }
    return result;

        //console.log(sURLVariables.length);
    /*for (var i = 0; i < sURLVariables.length; i++) {
        sParameterName = sURLVariables[i].split('=');
        console.log(i);
        if (sParameterName[0] === sParam) {
            return sParameterName[1] === undefined ? true : sParameterName[1];
        }else {
            return false;
        } 
     }*/ 
}

engine.open = (url) => {
    $('#viewport').remove();
    var viewport = document.createElement('iframe');
    viewport.id = "viewport";
    viewport.src = url;
    $(viewport).on('load', () => {
       document.title = document.getElementById("viewport").contentDocument.title;
    });
    $("body")[0].appendChild(viewport);

    $('#viewport').css('left', engine.left + "px");
    $('#viewport').css('top', engine.top + "px");
    $('#viewport').css('width', ($(window).width() - engine.right - engine.left) + "px");
    $('#viewport').css('height', ($(window).height() - engine.bottom - engine.top) + "px");
}
