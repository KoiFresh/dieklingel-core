class xPage extends HTMLElement {
    constructor() {
        super();
    }

    connectedCallback() {
        $(this).load($(this).attr("content"));
        //$(this).attr("src",$(this).attr("content"));
        console.log("x-page");
    }
}
customElements.define('x-page', xPage);