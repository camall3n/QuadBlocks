function setValue(id, val)
{
    document.getElementById(id).innerHTML=val;
    
    return true;
}

function getValue(id)
{
    return document.getElementById(id).innerHTML;
}

function setVisible(id, visible)
{
    if (visible) {
        document.getElementById(id).style.visibility="visible";
    }
    else {
        document.getElementById(id).style.visibility="hidden";
    }
    
    return true;
}

function selectNextElement(id)
{
    var list = document.getElementById(id).getElementsByTagName("li");
    
    var i;
    for (i=0; i<list.length; i++) {
        var div = list[i].getElementsByTagName("div")[0];
        var classStr = div.getAttribute("class");
        if (classStr =="selected" || classStr == "clicked") {
            div.setAttribute("class","");
            break;
        }
    }
    i = (i+1) % list.length;
    var div = list[i].getElementsByTagName("div")[0];
    div.setAttribute("class", "selected");
    
    return true;
}

function selectPrevElement(id)
{
    var list = document.getElementById(id).getElementsByTagName("li");
    
    var i;
    for (i=0; i<list.length; i++) {
        var div = list[i].getElementsByTagName("div")[0];
        var classStr = div.getAttribute("class");
        if (classStr =="selected" || classStr == "clicked") {
            div.setAttribute("class","");
            break;
        }
    }
    i = (i+list.length-1) % list.length;
    var div = list[i].getElementsByTagName("div")[0];
    div.setAttribute("class", "selected");
    
    return true;
}

function selectElement(id, selection)
{
    var list = document.getElementById(id).getElementsByTagName("li");
    
    var i;
    for (i=0; i<list.length; i++) {
        var div = list[i].getElementsByTagName("div")[0];
        var classStr = div.getAttribute("class");
        if (classStr =="selected" || classStr == "clicked") {
            div.setAttribute("class","");
            break;
        }
    }
    var div = list[selection].getElementsByTagName("div")[0];
    div.setAttribute("class", "selected");
    
    return true;
}

function clickSelectedElement(id)
{
    var list = document.getElementById(id).getElementsByTagName("li");
    
    var i;
    for (i=0; i<list.length; i++) {
        var div = list[i].getElementsByTagName("div")[0];
        var classStr = div.getAttribute("class");
        if (classStr =="selected") {
            div.setAttribute("class","clicked");
            break;
        }
    }
    return true;
}
function unClickSelectedElement(id)
{
    var list = document.getElementById(id).getElementsByTagName("li");
    
    var i;
    for (i=0; i<list.length; i++) {
        var div = list[i].getElementsByTagName("div")[0];
        var classStr = div.getAttribute("class");
        if (classStr=="clicked") {
            div.setAttribute("class","selected");
            break;
        }
    }
    return true;
}

function getSelectedElement(id)
{
    var list = document.getElementById(id).getElementsByTagName("li");
    
    var i;
    for (i=0; i<list.length; i++) {
        var div = list[i].getElementsByTagName("div")[0];
        var classStr = div.getAttribute("class");
        if (classStr =="selected" || classStr == "clicked") {
            break;
        }
    }
    return i;
}