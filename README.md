luddite
=======

Luddite Game Engine derived from bits of my ludum dare code.

Originally was meant to be portable but I'm only really using it on iOS and it's causing a lot
of build headaches keeping desktop/windows working. So now the goal is to make it ios-only but
it should be pretty easy to port if the need arises.

No real docs since this is too early to be useful, these are mostly notes to myself:

Steps to set up a project:
# Create a new iOS app project (such as the GL game template)
# Create a workspace (with cocoapods or just straight up)
# Drag the "luddite" project to the workspace
# Rename classes that will have luddite code visible to .mm (FIXME: could guard this in headers)
# Under app target, go to 'Link Binary with Libraries' and add the libluddite.a as a dependency
