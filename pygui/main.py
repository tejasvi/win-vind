import wx

class App(wx.App):
    def OnInit(self):
        frame = wx.Frame(None, wx.ID_ANY, 'Hello WorldA', size=(400, 300))

        panel = wx.Panel(frame, wx.ID_ANY)

        wx.StaticText(panel, wx.ID_ANY, 'HelloWorldTEXT')

        frame.Show()

        return True

if __name__ == '__main__':
    app = App(0)
    app.MainLoop()