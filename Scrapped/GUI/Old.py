gasLabel.grid(
    row = 0, 
    column = 0, 
    columnspan = 2,
    sticky = "ew")
gas = ttk.Progressbar(
    root, 
    orient="vertical", 
    mode="determinate", 
    maximum=255,)
gas.grid(
    row = 1, 
    column = 0,
    sticky = "ew",)

clutch = ttk.Progressbar(
    root,
    orient="vertical",
    mode="determinate",
    maximum=255,)
clutch.grid(
    row = 1, 
    column = 2)

def handle_data(data):
    value = data[1]  # Make it so this can be changed
    gas["value"] = value  # Update the bar

filter = hid.HidDeviceFilter(vendor_id=0x2341)
devices = filter.get_devices()

if devices:
    device = devices[0]
    device.open()
    device.set_raw_data_handler(handle_data)
else:
    print("No HID device found.")

root.mainloop()
