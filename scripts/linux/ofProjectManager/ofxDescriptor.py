import sys
import amara
from ofEnvironment import of_platform
class ofxDescriptor:
    """Abstraction of an addon install.xml file"""
    def __init__(self,name):
        self.name=name
       
        self.doc = amara.parse(name)
        
    includePaths = property(
        fget = lambda self: self.doc.install.add.include.path
    )

    def _get_libraries(self):
        try:
            libs=[]
            for lib in self.doc.install.add.link.lib:
                if hasattr(lib,"os"):
                    if str(lib.os) == of_platform:
                        libs.append(lib)
            return libs
                     
        except:
            return []
    libraries = property(
        fget = _get_libraries
    )
         
    folders = property(
        fget = lambda self: self.doc.install.add.src.folder
    )   