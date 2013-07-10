function setValue(id, val)
{
    document.getElementById(id).innerHTML=val;
    
    return true;
}

function getValue(id)
{
    return document.getElementById(id).innerHTML;
}