
luddite
--------

Luddite is a lightweight 3D game engine for iOS. This originally started as bits of ludumdare basecode. There are some great choices (e.g. Unreal) if you are making game with a huge team, or platforms like Unity can work well for small teams if you're willing to completely buy into their platform.

Instead, luddite aims to be a small, focused engine that is appropriate for individual devs or small teams. It's not trying
to reinvent the wheel, and it's not adding any fancy or cutting edge features. It doesn't try to be too flexible or modular, if you're using this for a real project I'd assume you're willing to hack on the engine a bit to get what you need. 

Originally was meant to be portable but I'm only really using it on iOS and it's causing a lot of build headaches keeping desktop/windows working. So right now it's only supported
on iOS but there shouldn't be too much platform specific stuff, and one day I'll revisit support for other platforms.

![Normal Mapping](http://www.tapnik.com/luddite_nrm_mapping_sm.png)

Look, fancy normal mapping.

Maturity
------
This is not a stable or complete engine. There are huge parts missing, and there are other fundamental parts that are slow or wrong or untested. 

That said, I have already shipped one game with it:

![Droppyship Screenshot](http://www.tapnik.com/images/droppyship-featured.png)

[Droppyship](http://www.tapnik.com/droppyship/)

[Droppyship on the App Store](https://itunes.apple.com/us/app/droppyship/id820172550?mt=8)

No real docs since this is too early to be useful.

Tools
-----

gfxpack - Simple utility to pack fonts and small images (like UI icons) into images.
