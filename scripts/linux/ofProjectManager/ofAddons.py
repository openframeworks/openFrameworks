import ofxDescriptor
import os
from ofEnvironment import of_root

class ofAddons:
    def __init__(self):
        self.addons = os.listdir(of_root + "addons/")