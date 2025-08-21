package com.orionsort.android

import org.libsdl.app.SDLActivity

class MainActivity : SDLActivity() {
    override fun getLibraries(): Array<out String?>? {
        return arrayOf("SDL3", "oboe", "openal", "OrionSort");
    }
}