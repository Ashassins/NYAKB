from typing import List, Tuple
from PIL import Image

def px2u16(px: List[int]) -> int:
    r = (px[0] / 255) * ((1 << 6 - 1) - 1)
    g = (px[1] / 255) * ((1 << 7 - 1) - 1)
    b = (px[2] / 255) * ((1 << 6 - 1) - 1)
    rv = int(r)
    rv = rv << 6 | int(g)
    rv = rv << 5 | int(b)
    return rv


def load_img(path: str) -> Tuple[List[int], Tuple[int, int]]:
    im = Image.open(path)
    px = im.load()
    img = []
    for i in reversed(range(im.size[1])):
        for j in reversed(range(im.size[0])):
            img.append(px2u16(px[j, i]))
    return img, im.size

def dump_img_arr(path: str) -> Tuple[str, str]:
    img, img_sz = load_img(path)
    name = path.split("/")[-1].split(".")[0].lower()
    c_str_data = ""
    c_str_data += f"const uint16_t {name}[{len(img)}] = {{\n\t"
    for i, px in enumerate(img):
        c_str_data += f"{hex(px)}, "
        if i % 5 == 4:
            c_str_data += "\n\t"

    c_str_data += "\n};\n"
    c_str_data += f"const uint8_t {name}_width = {img_sz[0]};\n"
    c_str_data += f"const uint8_t {name}_height = {img_sz[1]};\n"

    hdr_str = f"extern const uint16_t {name}[{len(img)}];\n"
    hdr_str += f"extern const uint8_t {name}_width;\n"
    hdr_str += f"extern const uint8_t {name}_height;\n"
    return c_str_data, hdr_str

if __name__ == "__main__":
    from sys import argv
    import os
    from os import path
    from glob import glob
    cwd = os.getcwd().split('/')

    proj_dir = "/"
    for p in cwd:
        proj_dir = path.join(proj_dir, p)
        if p == "mini_project":
            break

    hdr = """#ifndef SPRITE_DATA_H
#define SPRITE_DATA_H
#include <stdint.h>"""
    c_file = '#include "sprite_data.h"\n'

    files = sorted(glob(path.join(proj_dir, "Assets", "*.png")))


    for f in files:
        c_data, hdr_data = dump_img_arr(f)
        hdr += "\n" + hdr_data
        c_file += "\n" + c_data

    hdr += "\n#endif"
    hdr_path = path.join(proj_dir, "inc", "sprite_data.h")
    with open(hdr_path, "w") as f:
        f.write(hdr)

    src_path = path.join(proj_dir, "src", "sprite_data.c")
    with open(src_path, "w") as f:
        f.write(c_file)
