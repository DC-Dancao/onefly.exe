# -*- coding: utf-8 -*-
from __future__ import unicode_literals
import os.path

import plistlib
def read_plist(path):
	with open(path, 'rb') as f:
		return plistlib.load(f)

 
                                    
 

                                                               

                                                                             
               
 
                                                                             

                                                                                
server_enabled = 'server' in defines

application_client = defines.get('client', 'DDNet.app')
appname_client = os.path.basename(application_client)
if server_enabled:
	application_server = defines.get('server', 'DDNet-Server.app')
	appname_server = os.path.basename(application_server)

def icon_from_app(app_path):
	plist_path = os.path.join(app_path, 'Contents', 'Info.plist')
	plist = read_plist(plist_path)
	icon_name = plist['CFBundleIconFile']
	icon_root,icon_ext = os.path.splitext(icon_name)
	if not icon_ext:
		icon_ext = '.icns'
	icon_name = icon_root + icon_ext
	return os.path.join(app_path, 'Contents', 'Resources', icon_name)

                                                                                

                                           
                        

                                              
                       

                                          
format = defines.get('format', 'UDBZ')

                                 
compression_level = 9

             
size = defines.get('size', None)

                  
files = [ application_client ]
if server_enabled:
	files.append(application_server)

                    
symlinks = { 'Applications': '/Applications' }

               
                          

                                
hide_extension = [ appname_client ]
if server_enabled:
	hide_extension.append(appname_server)

             
 
                                                                                
                                                                                
                                                                             
                          
 
                            
badge_icon = icon_from_app(application_client)

                        
if server_enabled:
	icon_locations = {
		appname_client: (128, 288),
		appname_server: (272, 288),
		'Applications': (512, 288)
	}
else:
	icon_locations = {
		appname_client: (200, 288),
		'Applications': (440, 288)
	}

                                                                                

            
 
                                                   
 
                                           
                                                                         
                                                                
                                                            
                                                           
                                  
                                           
                                                                      
                                                 
 
                                                                         
                                                                         
            
 
                                                                        
                                                 
background = defines.get('background', 'builtin-arrow')

show_status_bar = False
show_tab_view = False
show_toolbar = False
show_pathbar = False
show_sidebar = False
sidebar_width = 180

                                            
window_rect = ((100, 100), (640, 444))

                                         
 
                
                
                  
                
 
default_view = 'icon-view'

                            
show_icon_preview = False

                                                                            
                                                
include_icon_view_settings = 'auto'
include_list_view_settings = 'auto'

                                                                                

arrange_by = None
grid_offset = (0, 0)
grid_spacing = 100
scroll_position = (0, 0)
label_pos = 'bottom'             
text_size = 16
icon_size = 118

                                                                                

                              
 
        
                 
                
              
                    
        
        
         
           
            
 
list_icon_size = 16
list_text_size = 12
list_scroll_position = (0, 0)
list_sort_by = 'name'
list_use_relative_dates = True
list_calculate_all_sizes = (False,)
list_columns = ('name', 'date-modified', 'size', 'kind', 'date-added')
list_column_widths = {
	'name': 300,
	'date-modified': 181,
	'date-created': 181,
	'date-added': 181,
	'date-last-opened': 181,
	'size': 97,
	'kind': 115,
	'label': 100,
	'version': 75,
	'comments': 300,
	}
list_column_sort_directions = {
	'name': 'ascending',
	'date-modified': 'descending',
	'date-created': 'descending',
	'date-added': 'descending',
	'date-last-opened': 'descending',
	'size': 'descending',
	'kind': 'ascending',
	'label': 'ascending',
	'version': 'ascending',
	'comments': 'ascending',
	}
