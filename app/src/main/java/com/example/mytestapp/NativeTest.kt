package com.example.mytestapp

class NativeTest {
    companion object {
        init {
            System.loadLibrary("mynativelib")
        }
    }

    external fun addTrack(trackID: Int, filepath: String)
    external fun nextFrame(trackID: Int)
}