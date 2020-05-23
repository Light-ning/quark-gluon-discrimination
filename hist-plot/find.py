from array import array
import re, sys, os, math

def main(path, tags):
    #NOTE given a path and tags (to identify the right dataset),
    #return the list of all the merged 'tree' files
    #NOTE 'path' should be the path to the gridOutput directory
    tags.append(".root")


    path+='/'
    fileList = []
    for f in os.listdir(path):
        if not os.path.isfile( os.path.join(path,f) ):
            for g in os.listdir( os.path.join(path,f) ):
                passed = True
                for tag in tags:
                    if '=' in tag:
                        if not any(splitTag in g for splitTag in list(tag.split('='))):
                            passed = False
                    else:
                        if not tag in g:
                            passed = False
                if passed:
                    fileList.append( os.path.join(path,f,g) )

    fileList.sort()
    for i in fileList:
        print(i)
    return fileList

if __name__ == "__main__":
    path = "/eos/atlas/atlascerngroupdisk/phys-exotics/jdm/dibjet/FullRUN2/NewCleaningMC/QCD_MC16a"
    tags = ["user"]
    main(path, tags)
