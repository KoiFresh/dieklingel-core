class xView extends HTMLElement {
    timeout = undefined;

    constructor() {
        super();
    }

    connectedCallback() {
        //$(this).load($(this).attr("content"));
        $(this).css("background-color", "#000000");
        $(this).css("position", "absolute");
        $(this).css("width", "100vw");
        $(this).css("height", "100vh");
        $(this).click(this.hide);
        this.draw();
    }

    attributeChangedCallback() {
        this.draw();
    }

    draw() {
        let src = this.attribute("content", "");
        $(this).load(src);
    }

    show() {
        $(this).css("display", "block");
        eval(this.attribute("onshow", "() => {}"))();
    }    

    hide() {
        $(this).css("display", "none");
        eval(this.attribute("onhide", "() => {}"))();

        let timeout = $(this).attr("timeout");
        clearTimeout(this.timeout);
        if(timeout > 0) {
            this.timeout = setTimeout(() => {
                this.show();
            }, timeout);
        }
    }

    attribute(name, value) {
        let attr = $(this).attr(name);
        if(typeof attr === 'undefined' || attr === false) 
        {
            attr = value;
        }
        return attr;
    }
}
customElements.define('x-view', xView);