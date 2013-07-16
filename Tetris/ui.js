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