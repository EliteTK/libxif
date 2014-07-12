libxif
======
The XIF file format library.

Motive
------
I had just started working with xcb, and realised that if I was going to start
throwing around xcb_image_t left and right when it comes to different programs,
I would need an easy way to store the entire type without having to faff about
with xcb_image_create() every time.

I simply decided that the best idea would be to create a simple file format
which would be able to store any of the 3 xcb_image_t formats.

Design
------
XIF (v0) has a 15 byte header which is formatted as such:
[3 byte signature "XIF"]
[2 byte width]
[2 byte height]
[1 byte depth]
[1 byte pad]
[1 byte unit]
[4 byte plane_mask]
[1 byte bitfield
{4 bit version}
{2 bit format}
{1 bit byte_order}
{1 bit bit_order}]

The endianness of the file is entirely dependant on how it was stored in memory
on the PC it was written with. This might change in the future if I ever decide
it to be necessary. (If anyone ever takes interest in this.)

Expansion
---------
I don't plan on expanding this, this is unless suddenly an outburst of 100
people start begging me for it. (I'm looking at you #xcb.)
