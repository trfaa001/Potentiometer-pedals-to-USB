import tkinter as tk
import customtkinter
from pywinusb import hid

customtkinter.set_appearance_mode("System")
customtkinter.set_default_color_theme("blue")

class App(customtkinter.CTk):
    def __init__(self):
        super().__init__()

        self.title("Pedal thingy") #Window name. to be changed
        self.geometry(f"{600}x{300}")

        #Configure grid layout
        self.grid_columnconfigure((0, 1), weight=1)
        self.grid_rowconfigure((0, 1), weight=1)

        #Create pedal frame
        self.pedalFrame = customtkinter.CTkFrame(self)
        self.pedalFrame.grid(row=0, column=0, padx=(20, 20), pady=(20, 0), sticky="nsew")
        self.pedalFrame.grid_columnconfigure((0, 1, 2), weight=1)
        self.pedalFrame.grid_rowconfigure((0, 1, 2, 3, 4, 5), weight=1)

        #Create pedal labels
        self.clutchLabel = customtkinter.CTkLabel(master=self.pedalFrame, text="Clutch")
        self.clutchLabel.grid(row=0, column=0, columnspan=1, padx=10, pady=10, sticky="n")

        self.brakeLabel = customtkinter.CTkLabel(master=self.pedalFrame, text="Brake")
        self.brakeLabel.grid(row=0, column=1, columnspan=1, padx=10, pady=10, sticky="n")

        self.gasLabel = customtkinter.CTkLabel(master=self.pedalFrame, text="Gas")
        self.gasLabel.grid(row=0, column=2, columnspan=1, padx=10, pady=10, sticky="n")

        #Create pedal bars
        self.clutchBar = customtkinter.CTkProgressBar(self.pedalFrame, orientation="vertical")
        self.clutchBar.grid(row=1, column=0, rowspan=5, padx=(10, 10), pady=(10, 10), sticky="")

        self.brakeBar = customtkinter.CTkProgressBar(self.pedalFrame, orientation="vertical")
        self.brakeBar.grid(row=1, column=1, rowspan=5, padx=(10, 10), pady=(10, 10), sticky="")

        self.gasBar = customtkinter.CTkProgressBar(self.pedalFrame, orientation="vertical")
        self.gasBar.grid(row=1, column=2, rowspan=5, padx=(10, 10), pady=(10, 10), sticky="")


        #Create option frame
        self.optionFrame = customtkinter.CTkFrame(self)
        self.optionFrame.grid(row=0, column=1, padx=(20, 20), pady=(20, 0), sticky="nsew")
        self.optionFrame.grid_columnconfigure(0, weight=1)
        self.optionFrame.grid_rowconfigure(4, weight=1)


        self.optionmenu_1 = customtkinter.CTkOptionMenu(self.optionFrame, dynamic_resizing=False,
                                                values=["Value 1", "Value 2", "Value Long Long Long"])
        self.optionmenu_1.grid(row=4, column=0, padx=20, pady=(20, 10), sticky="")



if __name__ == "__main__":
    app = App()
    app.mainloop()