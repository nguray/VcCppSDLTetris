import sys
import os

def data_to_uint32_header(wav_path):
    if not os.path.exists(wav_path):
        print(f"Error: {wav_path} not found.")
        return

    file_name = os.path.basename(wav_path)
    buff_name = file_name.replace('.', '_')

    with open(wav_path, "rb") as f:
        data = f.read()

    total_bytes = len(data)
    
    with open(f"{buff_name}.h", "w") as f:
        #f.write("#pragma once\n")
        #f.write("#include <cstdint>\n\n")
        f.write(f"// Original file size: {total_bytes} bytes\n")
        f.write(f"uint32_t {buff_name}_len = {total_bytes};\n\n")
        f.write(f"alignas(16) uint32_t {buff_name}[] = {{ \n    ")
        
        count = 0
        # Process data in 4-byte (32-bit) chunks
        for i in range(0, total_bytes, 4):
            chunk = data[i:i+4]
            # Pad with zeros if vv file length isn't perfectly divisible by 4
            if len(chunk) < 4:
                chunk = chunk + b'\x00' * (4 - len(chunk))
            
            # Pack as little-endian integer
            val = int.from_bytes(chunk, byteorder='little')
            f.write(f"0x{val:08X}, ")
            count += 1
            
            # Wrap rows every 6 elements for neatness
            if count % 6 == 0:
                f.write("\n    ")
                
        f.write("\n};\n")
    print(f"Successfully generated {buff_name}.h")

# Run it
if __name__ == "__main__":
    data_path = sys.argv[1]
    data_to_uint32_header(data_path)



