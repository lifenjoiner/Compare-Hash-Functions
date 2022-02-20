//
$WshShell   = new ActiveXObject('WScript.Shell');
$cmd        = function(s){return /*controllable*/ $WshShell.Exec(s);}
$run        = function(s,t,r){return $WshShell.Run(s, t?t:10, r?r:false); /*?!url*/}
/*******/
try{WScript.StdOut.WriteLine()}catch(e){ //^_^ CLI. compiled?
    $run('cscript.exe //nologo "'+ WScript.ScriptFullName +'"');
    WScript.Quit();
}
//
$in     = WScript.StdIn;
$out    = WScript.StdOut;
$err    = WScript.StdErr;
$echo   = function(s){$out.Write(s)}
$msg    = function(s){$echo(s+'\n')}
$argv   = WScript.Arguments;
$is_dbg = $argv.Named.Exists('d');
$dsg    = function(s){ if ($is_dbg) {$msg(s)} }
//
$fso = new ActiveXObject('Scripting.FileSystemObject');
//
$textstream = function(p,m,n,t){return $fso.OpenTextFile(p, m?m:1, n?n:false, t?t:0)}
$newstream  = function(p,t){return $textstream(p, 2, true, t?t:0)}
$getstream_r  = function(p,t){return $textstream(p, 1, false, t?t:0)}
//
$readall= function(s,t){
    // 0:ANSI, -1:unicode, -2:system
    var f = $getstream_r(s, t?t:0);
    var s = f.ReadAll();
    f.Close();
    return s;
}

/*******/
function textline2array(text) {
    text = text.replace(/\r\n/mg, "\n");
    text = text.replace(/\r/mg, "\n");
    return text.split("\n");
}

function array2beautytext(lines) {
    lines = lines.join("\n");
    return lines.replace(/\n\n\n/mg, "\n\n");
}


function count_items(items){
    if (!items) return [];
    var items_u = {};
    var items_r = [];
    var L = items.length;
    // count
    for (var i = 0; i < L; i++) {
        if (!!items[i]) {
            if (items[i] in items_u) { // 400 times faster
                items_u[ items[i] ] += 1;
            }
            else {
                items_u[ items[i] ] = 1;
            }
        }
    }
    // get result
    for (var i = 0; i < L; i++) {
        if (!!items[i]) {
            items_r.push( items[i] +'\t'+ items_u[items[i]] );
        }
        else {
            items_r.push( items[i] ); // keep
        }
    }
    return items_r;
}

function get_result(file){
    var text;
    $dsg(file);
    text = $readall(file, 0);
    text = textline2array(text);
    text = count_items(text);
    text = array2beautytext(text);
    return text;
}

function update_result(file){
    var t = get_result(file);
    if (t) {
        var f = $newstream(file);
        f.Write(t);
        f.Close();
    }
}

$msg(new Date() +" [Start]");
if ($is_dbg) {
    $dsg( get_result($argv(0)) );
}else {
    update_result( $argv(0) );
}
$msg(new Date() +" [Finished]");
