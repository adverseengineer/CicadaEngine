from PIL import Image
import io
import numpy as np
from struct import unpack, pack

image = Image.open("NSInteractiveGraphicsS24/gw.png")
array = np.frombuffer(image.tobytes(), dtype=np.float32)

row = []
hexForm = "{:02X}"
for elem in reversed(array):
	asUint = unpack('I', elem)[0]
	r = hexForm.format(asUint & 0xFF)
	g = hexForm.format((asUint >> 8) & 0xFF)
	b = hexForm.format((asUint >> 16) & 0xFF)
	a = hexForm.format((asUint >> 24) & 0xFF)
	row.append(f"0x{a}")
	row.append(f"0x{b}")
	row.append(f"0x{g}")
	row.append(f"0x{r}")
	if len(row) >= (4*64):
		for byte in reversed(row):
			print(f"{byte}, ", end='')
		print("")
		row = []
	#print("{:02X}".format(asUint))
