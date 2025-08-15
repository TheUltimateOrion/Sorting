package com.theultimateorion.orionsort_android

import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity

class MainActivity : AppCompatActivity() {
    companion object {
        init {
            System.loadLibrary("OrionSort")
        }
    }

    private external fun nativeInit()

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        nativeInit()
    }
}
