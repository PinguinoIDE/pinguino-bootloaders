#!/usr/bin/python
# -*- coding: iso-8859-1 -*-

########################################################################
#                                                                      #
#   Wistiti - Pinguino's Best Friend                                   #
#   8-bit USB Bootloader Factory                                       #
#   Compile 8-bit Pinguino USB Bootloader                              #
#   Author:	2012 - Régis Blanchot <rblanchot@gmail.com>                #
#   Last update : 26 Aug. 2016                                         #
#                                                                      #
#   Usage: ./wiztiti.py                                                #
#                                                                      #
#   This file is part of Pinguino Project (http://www.pinguino.cc)     #
#   Released under the LGPL license (www.gnu.org/licenses/lgpl.html)   #
#                                                                      #
########################################################################

try:
    import wx
except ImportError:
    raise ImportError,"The wxPython module is required to run this program."

try:
    import usb
except ImportError:
    raise ImportError,"The PyUSB module is required to run this program."

from wx.lib.buttons import GenBitmapTextButton
from subprocess import Popen,PIPE,STDOUT


########################################################################
class MainPanel(wx.Panel):
    """
    Compile a USB Bulk bootloader with various options.
    After the compilation, the bootloader can be programmed
    (see Program panel) into the target microcontroller.
    """
 
    #-------------------------------------------------------------------
    def __init__(self, parent):
        """Constructor"""
        self.panel = wx.Panel.__init__(self, parent=parent)
        self.SetBackgroundStyle(wx.BG_STYLE_CUSTOM)

        compilerList = [  
            'SDCC',   'XC8'
        ]

        devicesList = [  
            '16f1459',
            '18f13k50', '18f14k50',
            '18f2455', '18f2550', '18f25k50', '18f26j50', '18f26j53', '18f27j53',
            '18f4455', '18f4550', '18f45k50', '18f46j50', '18f46j53', '18f47j53'
        ]

        oscillatorList = [
            '4',    '8',    '12',    '16',    '20',    '24',    '40',   '48',
            'INTOSC'
        ]

        optionsList = [
            "String Descriptor", "Low Speed USB", 
            "Low Voltage Programming", "Ext. 32768Hz Oscillator",
            "Watchdog Enabled"
        ]

        buttonsList = [
                ('img/runw.png',  'Compile the bootloader code source', self.OnCompile),
                ('img/board.png', 'Program (ICSP) the bootloader on the chip', self.OnProgram),
                ('img/open.png',  'Open a Hex file',    self.OnOpen),
                ('img/dwn.png',   'Upload a Hex File on the Chip with the bootloader\'s help',  self.OnUpload),
                ('img/help.png',  'Get some explanations',    self.OnHelp),
                ('img/exit.png',  'Quit Wiztiti program',    self.OnQuit)
        ]

        sizerList = []                  # list of all sizers
        
        # --------------------------------------------------------------
        # Compiler
        # --------------------------------------------------------------

        CmpBox = wx.StaticBox( self, -1, label = u'Compiler ')
        boldfont = CmpBox.GetFont()
        boldfont.SetWeight(wx.BOLD)
        CmpBox.SetFont(boldfont)
        sizerList.append ( wx.StaticBoxSizer ( CmpBox, wx.VERTICAL ) )

        # Create CheckBoxes for each supported processor
        old_col = 0
        grid = []
        grid.append ( wx.BoxSizer ( wx.HORIZONTAL ) )

        self.checkboxCmpList = []

        for i in range(len(compilerList)):
            # list of all checkboxes
            self.checkboxCmpList.append( wx.CheckBox ( self, id = i, label = compilerList[i] ))

            new_col = i / 5
            if ( new_col > old_col ) :
                old_col = new_col
                grid.append ( wx.BoxSizer ( wx.HORIZONTAL ) )

            grid[-1].Add ( self.checkboxCmpList[-1], 0, wx.ALL|wx.EXPAND, 5 )

        for i in range(len(grid)):
            sizerList[-1].Add ( grid[i], 0, wx.ALL|wx.EXPAND, 5 )

        # --------------------------------------------------------------
        # Microcontroller
        # --------------------------------------------------------------

        DevBox = wx.StaticBox( self, -1, label = u'PIC Microcontroller Target ')
        boldfont = DevBox.GetFont()
        boldfont.SetWeight(wx.BOLD)
        DevBox.SetFont(boldfont)
        sizerList.append ( wx.StaticBoxSizer ( DevBox, wx.VERTICAL ) )

        # Create CheckBoxes for each supported processor
        old_col = 0
        grid = []
        grid.append ( wx.BoxSizer ( wx.HORIZONTAL ) )

        self.checkboxDevList = []

        for i in range(len(devicesList)):
            # list of all checkboxes
            self.checkboxDevList.append( wx.CheckBox ( self, id = i, label = devicesList[i] ))

            new_col = i / 5
            if ( new_col > old_col ) :
                old_col = new_col
                grid.append ( wx.BoxSizer ( wx.HORIZONTAL ) )

            grid[-1].Add ( self.checkboxDevList[-1], 0, wx.ALL|wx.EXPAND, 5 )

        for i in range(len(grid)):
            sizerList[-1].Add ( grid[i], 0, wx.ALL|wx.EXPAND, 5 )

        # --------------------------------------------------------------
        # Oscillator
        # --------------------------------------------------------------

        OscBox = wx.StaticBox( self, -1, label = u'Oscillator\'s Frequency (MHz)')
        boldfont = OscBox.GetFont()
        boldfont.SetWeight(wx.BOLD)
        OscBox.SetFont(boldfont)
        sizerList.append ( wx.StaticBoxSizer ( OscBox, wx.VERTICAL ) )

        old_col = 0
        grid = []
        grid.append ( wx.BoxSizer ( wx.HORIZONTAL ) )

        self.checkboxOscList = []

        for i in range(len(oscillatorList)):
            # list of all checkboxes
            self.checkboxOscList.append( wx.CheckBox ( self, id = i , label = oscillatorList[i] ))

            new_col = i / 8
            if ( new_col > old_col ) :
                old_col = new_col
                grid.append ( wx.BoxSizer ( wx.HORIZONTAL ) )

            grid[-1].Add ( self.checkboxOscList[-1], 0, wx.ALL|wx.EXPAND, 5 )

        for i in range(len(grid)):
            sizerList[-1].Add ( grid[i], 0, wx.ALL|wx.EXPAND, 5 )

        # --------------------------------------------------------------
        # Options
        # --------------------------------------------------------------

        OptBox = wx.StaticBox( self, -1, label = u'Options')
        boldfont = OptBox.GetFont()
        boldfont.SetWeight(wx.BOLD)
        OptBox.SetFont(boldfont)
        sizerList.append ( wx.StaticBoxSizer ( OptBox, wx.VERTICAL ) )

        old_col = 0
        grid = []
        grid.append ( wx.BoxSizer ( wx.HORIZONTAL ) )

        self.checkboxOptList = []

        for i in range(len(optionsList)):
            # list of all checkboxes
            self.checkboxOptList.append( wx.CheckBox ( self, id = i, label = optionsList[i] ))

            new_col = i / 2
            if ( new_col > old_col ) :
                old_col = new_col
                grid.append ( wx.BoxSizer ( wx.HORIZONTAL ) )

            grid[-1].Add ( self.checkboxOptList[-1], 0, wx.ALL|wx.EXPAND, 5 )

        for i in range(len(grid)):
            sizerList[-1].Add ( grid[i], 0, wx.ALL|wx.EXPAND, 5 )

        # --------------------------------------------------------------
        # Buttons
        # --------------------------------------------------------------

        ButBox = wx.StaticBox( self, -1, label = u'Actions')
        boldfont = OptBox.GetFont()
        boldfont.SetWeight(wx.BOLD)
        ButBox.SetFont(boldfont)
        sizerList.append ( wx.StaticBoxSizer ( ButBox, wx.VERTICAL ) )

        old_col = 0
        grid = []
        grid.append ( wx.BoxSizer ( wx.HORIZONTAL ) )

        self.butList = []

        for i in range(len(buttonsList)):
            # list of all buttons
            img, lab, event = buttonsList[i]
            b = GenBitmapTextButton(self, i, wx.Bitmap(img),
                label = "", size = (48, 48) )
            b.SetBezelWidth(1)
            b.SetBackgroundColour('#DDDDDD')
            #b.SetHelpText(lab)
            b.SetToolTipString(lab)
            # Catch events
            b.Bind(wx.EVT_BUTTON, event)
            b.Bind(wx.EVT_ENTER_WINDOW, self.OnButtonEnter)
            b.Bind(wx.EVT_LEAVE_WINDOW, self.OnButtonLeave)
            self.butList.append( b )

            new_col = i / 6
            if ( new_col > old_col ) :
                old_col = new_col
                grid.append ( wx.BoxSizer ( wx.HORIZONTAL ) )

            grid[-1].Add ( self.butList[-1], 0, wx.ALL|wx.EXPAND, 5 )

        for i in range(len(grid)):
            sizerList[-1].Add ( grid[i], 0, wx.ALL|wx.EXPAND, 5 )

        # --------------------------------------------------------------
        # Sizer
        # --------------------------------------------------------------

        topSizer = wx.BoxSizer ( wx.VERTICAL   )
        for i in range(len(sizerList)):
            topSizer.Add ( sizerList[i], 0, wx.ALL|wx.EXPAND, 5 )

        self.SetSizerAndFit ( topSizer )
        self.Show ( True )

    # Button control
    #----------------------------------------------------------------------
    def OnButtonEnter(self, event):
        obj =  event.GetEventObject()
        obj.SetBackgroundColour('#CCCCCC')
        obj.Refresh()

    def OnButtonLeave(self, event):
        obj =  event.GetEventObject()
        obj.SetBackgroundColour('#DDDDDD')
        obj.Refresh()
  
    # Add a picture to the background
    #----------------------------------------------------------------------
    def OnEraseBackground(self, event):
        dc = event.GetDC()
        if not dc:
            dc = wx.ClientDC(self)
            rect = self.GetUpdateRegion().GetBox()
            dc.SetClippingRect(rect)
        dc.Clear()
        bmp = wx.Bitmap("img/wiztiti.jpg")
        dc.DrawBitmap(bmp, 0, 0)
 
    # If User press Compile button
    #----------------------------------------------------------------------
    def OnCompile(self,event):

        opt = []
        # read all options
        for i in range(len(self.checkboxOptList)):
            opt.append(self.checkboxOptList[i].GetValue() * 1)

        # 1st pass : was at least 1 proc. selected ?
        p = 0
        for i in range(len(self.checkboxDevList)):
            if (self.checkboxDevList[i].GetValue()):
                break
            else :
                p = p + 1

        if (p == len(self.checkboxDevList)):
            dlg = wx.MessageDialog(self, 'Please choose at least one Microncontroller in the list.', 'Error', wx.OK|wx.ICON_ERROR)
            dlg.ShowModal()
            dlg.Destroy()
            return

        # 2nd pass : was at least 1 osc. selected ?
        p = 0
        for i in range(len(self.checkboxOscList)):
            if (self.checkboxOscList[i].GetValue()):
                break
            else :
                p = p + 1

        if (p == len(self.checkboxOscList)):
            dlg = wx.MessageDialog(self, 'Please choose at least one Oscillator in the list.', 'Error', wx.OK|wx.ICON_ERROR)
            dlg.ShowModal()
            dlg.Destroy()
            return

        # 3rd pass : was at least 1 compiler selected ?
        p = 0
        for i in range(len(self.checkboxCmpList)):
            if (self.checkboxCmpList[i].GetValue()):
                break
            else :
                p = p + 1

        if (p == len(self.checkboxCmpList)):
            dlg = wx.MessageDialog(self, 'Please choose at least one Compiler in the list.', 'Error', wx.OK|wx.ICON_ERROR)
            dlg.ShowModal()
            dlg.Destroy()
            return

        # 4th pass : compile bootloader for all Compiler, MCU and Osc. value
        for c in range(len(self.checkboxCmpList)):
            if (self.checkboxCmpList[c].GetValue()):
                comp = self.checkboxCmpList[c].GetLabel()
                print comp, 
                for d in range(len(self.checkboxDevList)):
                    if (self.checkboxDevList[d].GetValue()):
                        proc = self.checkboxDevList[d].GetLabel()
                        print proc, 
                        for o in range(len(self.checkboxOscList)):
                            if (self.checkboxOscList[o].GetValue()):
                                osc = self.checkboxOscList[o].GetLabel()
                                print osc,
                                self.Compile(comp, proc, osc, opt[0], opt[1], opt[2], opt[3])
                                print
                        
    # If User press Program button
    #----------------------------------------------------------------------
    def OnProgram(self,event):
        """Write Hex file with an In-Circuit Serial Programmer"""

    # If User press Open button
    #----------------------------------------------------------------------
    def OnOpen(self,event):
        """Open a file"""
        self.dirname = ''
        dlg = wx.FileDialog(self, "Choose a file", "hex", "", "*.hex", wx.OPEN)
        if dlg.ShowModal() == wx.ID_OK:
            self.filename = dlg.GetFilename()
            self.dirname = dlg.GetDirectory()
            f = open(os.path.join(self.dirname, self.filename), 'r')
            self.control.SetValue(f.read())
            f.close()
        dlg.Destroy()

    # If User press Upload button
    #----------------------------------------------------------------------
    def OnUpload(self,event):
        """Upload Hex file with the bootloader"""

    # If User press Help button
    #----------------------------------------------------------------------
    def OnHelp(self,event):
        """Display Help"""

        help_what    = "What is a Bootloader ?\r\n\r\nBootloaders can program microcontrollers remaining flash memory without necessity to use any external hardware (like ICD2 programmer). Bootloader receives data from PC through USB bus and writes it into the PIC microcontroller memory.\r\n\r\n"
        help_why     = "Why use one ?\r\n\r\nBootloaders don't require any Hardware changes in your device. It uses the standard USB interface of your microcontroller.\r\n\r\n"
        help_how     = "How does it work ?\r\n\r\nBootloaders utilize memory self-programming ability of PIC18F microcontrollers. Once burned into the microcontroller flash memory, a bootloader can repeatedly reprogram it without expensive chip writers or ICSP (In-Circuit Serial Programmer).\r\n\r\n"
        help_where   = "Where is the bootloader ?\r\n\r\nOur bootloader resides in the lower 3072 bytes (0x0C00) of the PIC microcontroller flash memory.\r\n\r\n"
        help_compile = "What does compile mean ?\r\n\r\nCompilation translates high-level programming language into microcontroller language. After the compilation bootloader can be programmed (upload) into the target microcontroller.\r\n\r\n"
        help_upload  = "Waht does upload mean ?\r\n\r\nWhen Upload button is pressed, the bootloader writes the user program into the target microcontroller.\r\n\r\n"
        help_program = "What does program mean ?\r\n\r\nWhen Program button is pressed, the bootloader is written into the target microcontroller. We use a Pinguino board as an ICSP Programmer to do it.\r\n\r\n"

        dlg = wx.MessageDialog(self, help_what + help_why + help_how + help_where + help_compile + help_upload + help_program,
                                'Help', wx.OK|wx.ICON_INFORMATION)
        dlg.ShowModal()
        dlg.Destroy()
  
    # If User press Quit button
    #----------------------------------------------------------------------
    def OnQuit(self,event):
        """Exit Wiztiti program"""
        self.Destroy()

    # Compile Bootloader(s) by calling Makefile
    #----------------------------------------------------------------------
    def Compile(self, comp, proc, osc, string, speed, lvp, watchdog):
        """Compile the bootloader code source"""
        output = Popen(args=[   "make",
                                "all",
                                "--makefile=Makefile",
                                "COMP=" + comp,
                                "PROC=" + proc,
                                "OSC=" + osc,
                                "STRINGDESC=%d" % string,
                                "SPEED=%d" % speed,
                                "VOLTAGE=%d" % lvp ],
                                stdout=PIPE,
                                stderr=STDOUT,
                                shell=False,        # redirect error stream to stdout
                                close_fds=False)
        output.wait() # or poll()
        #output.communicate()
        outputlines = output.stdout.readlines()
        error = ""
        for i in range(len(outputlines)):
            pos = outputlines[i].find("error:")
            if pos != -1:
                error = error + outputlines[i][pos+7:-1] + "\r\n"
        # Display a dialog only if an error occurs
        if error != "":
            dialog = wx.MessageDialog ( self, error, 'Error!', style = wx.OK )
            dialog.ShowModal()
            dialog.Destroy()

    # Upload Hex file on Pinguino Board
    #----------------------------------------------------------------------
    def Upload(self, proc, osc, hex_file):
        print "hex=" + hex_file
        #Gauge.SetValue(self.count)
 
########################################################################
class MainFrame(wx.Frame):
    """"""
 
    # Create a panel to house everything
    #-------------------------------------------------------------------
    def __init__(self):
        wx.Frame.__init__(self, None,
                            title = 'Wiztiti - Pinguino\'s Best Friend !',
                            size  = ( 600, 600 )) 

        panel = MainPanel(self)        

        ico = wx.Icon("img/wiztiti.ico", wx.BITMAP_TYPE_ICO)
        self.SetIcon(ico)
        self.Center()

########################################################################
class wiztiti(wx.App):
    """"""
 
    #-------------------------------------------------------------------
    def __init__(self, redirect=False, filename=None):
        """Constructor"""
        wx.App.__init__(self, redirect, filename)
        dlg = MainFrame()
        dlg.Show()
 
#----------------------------------------------------------------------
if __name__ == "__main__":
    app = wiztiti()
    app.MainLoop()
