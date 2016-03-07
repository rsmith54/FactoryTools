from xAH_config import xAH_config

basicEventSelectionDict = {
#todo larry

}

def config(alg , configDict = basicEventSelctionDict ) :
    c = xAH_config()
    c.setalg("BasicEventSelection", configDict)

