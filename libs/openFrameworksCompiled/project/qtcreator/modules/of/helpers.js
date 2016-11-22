var Process = loadExtension("qbs.Process");
var File = loadExtension("qbs.File");
var TextFile = loadExtension("qbs.TextFile");
var Environment = loadExtension("qbs.Environment");
var qbUtils = loadExtension("qbs");

function findCommand(){
    // check if it's unix
    if(File.exists("/usr/bin/find")){
        return "/usr/bin/find";
    }

    // else msys2, search unix find command
    var where = new Process();
    where.exec("where.exe", ['find'], true);
    if(where.exitCode()!==0){
        throw("error: There is a problem to detect the 'find' command."+where.exitCode());
    }

    while(true){
        var line = where.readLine();
        if(line!=="" && line!==undefined){
            var findPos = line.indexOf("usr\\bin\\find.exe");
            if (findPos > -1){
                return line;
            }
        }else{
            break;
        }
    }
    return "find";
}

function getSystemPath(){
    return Environment.getEnv("PATH");
}

function msys2root(){
    var msys2 = "";
	var systemPath = Environment.getEnv("PATH");
	if(systemPath === undefined){
        console.error("PATH is not defined")
        return msys2;
	}
	
	var where = new Process();
    where.exec("where.exe", ['find']); 
    if(where.exitCode()!==0){
        throw("error: There is a problem to detect the 'find' command.");
    }


    while(true){
        var line = where.readLine();
        if(line!=="" && line!==undefined){
            var findPos = line.indexOf("usr\\bin\\find.exe");
            if (findPos > -1){
                msys2 = line.slice(0,findPos);
                break;
            }
        }else{
            break;
        }
    }

	
    //console.error("PATH=>"+systemPath);
    var usrPos = systemPath.indexOf(msys2 + "usr\\bin");
    var mingw32Pos = systemPath.indexOf(msys2 + "mingw32\\bin");
	
    if( (usrPos === -1) || (mingw32Pos === -1) || (mingw32Pos > usrPos) ){
        console.error("PATH="+systemPath);
		throw("error : your PATH is incorrect. Please make sure that {MSYS2ROOT}\\mingw32\\bin;{MSYS2ROOT}\\usr\\bin is at the beginning of your PATH");
    }
	
	return msys2;
}


function listDir(dir){
    var ls = new Process();
    ls.exec("ls", [dir]);
    if(ls.exitCode()!==0){
        var error = ls.readStdErr();
        throw("error: " + error)
    }
    var line = ls.readLine();
    var ret = []
    while(line.trim()!==""){
        ret = ret.concat([line.trim()]);
        line = ls.readLine();
    }
    ls.close();
    return(ret)
}

function listDirsRecursive(dir){
    var ret = []
    if(!File.exists(dir)){
        return ret;
    }
    var find = new Process();
    var params = [dir,'-type','d'];
    find.exec(findCommand(), params)
    if(find.exitCode()!==0){
		var error = find.readStdErr();
		throw("error: " + error)
    }
    var line = find.readLine();
    while(line.trim()!==""){
        ret = ret.concat([line.trim()]);
        line = find.readLine();
    }
    find.close();
    return(ret)
}

function hasExtension(str, extension){
    var suffix = "." + extension;
    return str.indexOf(suffix, str.length - suffix.length) !== -1;
}

function pkgExists(pkg){
    var pkgconfig = new Process();
    pkgconfig.exec("pkg-config", ["--exists", pkg]);
    return pkgconfig.exitCode() === 0;
}

function findLibsRecursive(dir, platform, exclude){
    var ret = []
    if(!File.exists(dir)){
        return ret;
    }
    var libs = listDir(dir);
    for(libdir in libs){
        if(exclude.indexOf(libs[libdir])!==-1){
            continue;
        }

        var libpath = dir + '/' + libs[libdir] + '/lib/' + platform + "/"
        var find = new Process();
        find.exec('ls', [libpath])
        var line = find.readLine().trim();
        var libFiles = [];
        while(line!==""){
            libFiles.push(line);
            line = find.readLine().trim();
        }
        find.close();

        var staticLibs = libFiles.filter(function(lib){
            return hasExtension(lib, "a");
        });

        var sharedLibs = libFiles.filter(function(lib){
            return hasExtension(lib, "so") || hasExtension(lib, "dylib") || hasExtension(lib, "dll");
        });

        for(var idx in staticLibs){
            ret.push(libpath + staticLibs[idx])
        }

        for(var idx in sharedLibs){
            ret.push(libpath + sharedLibs[idx])
        }

        /*if(sharedLibs.length>0){
            ret.push("-L"+libpath);
        }

        for(var idx in sharedLibs){
            var lib = sharedLibs[idx];
            if(lib.indexOf("lib")===0){
                lib = lib.substr(3);
            }
            lib = lib.split(".")[0];
            ret.push("-l" + lib)
        }*/
    }
    return(ret)
}

function findSourceRecursive(dir){
    var ret = []
    if(!File.exists(dir)){
        return ret;
    }
    var find = new Process();
    var params = [dir ,'-name', '*.cpp'
                  ,'-or', '-name', '*.h'
                  ,'-or', '-name', '*.hpp'
                  ,'-or', '-name', '*.cxx'
                  ,'-or', '-name', '*.cc'
                  ,'-or', '-name', '*.c++'
                  ,'-or', '-name', '*.s'
                  ,'-or', '-name', '*.S'
                  ,'-or', '-name', '*.c'
                  ,'-or', '-name', '*.inl'
                  ,'-or', '-name', '*.glsl'
                  ,'-or', '-name', '*.vert'
                  ,'-or', '-name', '*.frag'];
    find.exec(findCommand(), params);
    if(find.exitCode()!==0){
		var error = find.readStdErr();
		throw("error: " + error)
    }
    var line = find.readLine();
    while(line.trim()!==""){
        ret = ret.concat([line.trim()]);
        line = find.readLine();
    }
    find.close();
    return(ret)
}

function pkgconfig(pkgs,parameters){
    if(pkgs.length===0){
        return [];
    }
    var pkgconfig = new Process();
    var arguments = pkgs.concat(parameters);
    pkgconfig.exec('pkg-config', arguments);
    if(pkgconfig.exitCode()!==0){
        var error = pkgconfig.readStdErr();
        throw("error: " + error)
    }
    return pkgconfig.readLine().split(" ").filter(function(element){
        return element.trim()!=="";
    });
}

function addonIncludes(addon){
    var includes = listDirsRecursive(addon + '/src')
    try{
        var libs = listDir(addon + '/libs/');
        var libsIncludes = [];
        for(var lib in libs){
            var libpath = addon + '/libs/' + libs[lib];
            try{
                var include_paths = listDirsRecursive(libpath);
                libsIncludes = libsIncludes.concat(include_paths);
            }catch(e){}
        }
        if(File.exists(addon + '/libs') && libsIncludes.length==0){
            includes = includes.concat(listDirsRecursive(addon + '/libs'));
        }else{
            includes = includes.concat(libsIncludes);
        }
    }catch(e){ }
    return includes;
}

function addonSources(addon){
    var sources = findSourceRecursive(addon + '/src')
    try{
        sources = sources.concat(findSourceRecursive(addon + '/libs'));
    }catch(e){}
    return sources;
}

function parseConfig(configPath, variable, currentValue, platform, prefix){
    var values = currentValue.slice(0);
    try{
        var section = "";
        var addonconfig = new TextFile(configPath);
        while(!addonconfig.atEof()){
            var line = addonconfig.readLine().trim();
            var varValue;
            var lineVar;
            var valuesList;
            if(line==="" || line.startsWith("#")){
                continue;
            }
            
            if(line.indexOf(":") === line.length-1){
                section = line.substr(0,line.length-1);
            }else if(section === platform || section === "common" || platform === "all"){
                if(line.indexOf("+=") !== -1){
                    varValue = line.split("+=");
                    lineVar = varValue[0].trim();
                    if(lineVar === variable){
                        valuesList = varValue[1].split(" ").filter(function(element){
                            return element.trim() !== "";
                        });
                        if(prefix!==undefined){
                            valuesList = valuesList.map(function(element){
                                if(!FileInfo.isAbsolutePath(element)){
                                    return prefix + element;
                                }else{
                                    return element;
                                }
                            });
                        }
                        values = values.concat(valuesList)
                    }
                }else if(line.indexOf("=") !== -1){
                    varValue = line.split("=");
                    lineVar = varValue[0].trim();
                    if(lineVar === variable){
                        values = varValue[1].split(" ").filter(function(element){
                            return element.trim() !== "";
                        });
                        if(prefix!==undefined){
                            values = values.map(function(element){
                                if(!FileInfo.isAbsolutePath(element)){
                                    return prefix + element;
                                }else{
                                    return element;
                                }
                            });
                        }
                    }
                }
            }
        }
    }catch(e){
        //throw(e)
    }
    
    return values;
}

function parseAddonConfig(addonPath, variable, currentValue, platform, prefix){
    var configPath = addonPath + "/addon_config.mk";
    return parseConfig(configPath, variable, currentValue, platform, prefix);
}

function removeDuplicates(arrayName){
    var newArray=[];
    label:for(var i=0; i<arrayName.length;i++ ){  
        for(var j=0; j<newArray.length;j++ ){
            if(newArray[j]===arrayName[i])
            continue label;
        }
        newArray[newArray.length] = arrayName[i];
    }
    return newArray;
}

function normalize(path) {
    var BLANK = '';
    var SLASH = '/';
    var DOT = '.';
    var DOTS = DOT.concat(DOT);
    var SCHEME = '://';
    path = path.replace("\\","/");
    if (!path || path === SLASH) {
        return SLASH;
    }

    /*
       * for IE 6 & 7 - use path.charAt(i), not path[i]
       */
    var prependSlash = (path.charAt(0) == SLASH || path.charAt(0) == DOT);
    var target = [];
    var src;
    var scheme;        
    var parts;
    var token;
    
    if (path.indexOf(SCHEME) > 0) {
    
        parts = path.split(SCHEME);
        scheme = parts[0];
        src = parts[1].split(SLASH);
    } else {
    
        src = path.split(SLASH);
    }

    for (var i = 0; i < src.length; ++i) {
    
        token = src[i];
        
        if (token === DOTS) {
            target.pop();
        } else if (token !== BLANK && token !== DOT) {
            target.push(token);
        }
    }

    var result = target.join(SLASH).replace(/[\/]{2,}/g, SLASH);
          
    return (scheme ? scheme + SCHEME : '') + (prependSlash ? SLASH : BLANK) + result;
}

function absOFRoot(){
    if(FileInfo.isAbsolutePath(project.of_root)){
        return project.of_root;
    }else{
        return FileInfo.joinPaths(path, project.of_root);
    }
}

