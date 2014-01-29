import glob
import os
import sys

#______________________________________________________________________________
def get_unmerged_files(input_bases):
    unmerged_files = []
    for directory in input_bases:
        files_in_dir = glob.glob(os.path.join(directory, '*.root'))
        files_in_dir.sort()
        unmerged_files += files_in_dir
        ## actual number of files
        actual = len(files_in_dir)
        ## Extract the expected number of files from the last filename, 
        ## for example:
        ## 'hgg-2013Final8TeV_r12d-dph-j22-v1_noskim_0183.root'.split('_')
        ## ['hgg-2013Final8TeV', 'r12d-dph-j22-v1', 'noskim', '0183.root'][-1]
        ## '0183.root'[:4]
        ## '0183'.lstrip('0')
        ## int('183')
        ## 183 + 1
        ## 184
        expected = int(files_in_dir[-1].split('_')[-1][:4].lstrip('0')) + 1
        print >> sys.stderr, '%s: added %3d files' % (directory, actual),
        print >> sys.stderr, 'of %3d expected ...' % expected
    return unmerged_files
## get_unmerged_files