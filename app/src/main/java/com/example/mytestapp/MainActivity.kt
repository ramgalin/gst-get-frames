package com.example.mytestapp

import android.content.Context
import android.os.Bundle
import android.util.Log
import android.widget.Toast
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.Button
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Surface
import androidx.compose.material3.Text
import androidx.compose.ui.unit.dp
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.tooling.preview.Preview
import com.example.mytestapp.ui.theme.MytestappTheme
import androidx.compose.runtime.remember
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.getValue
import androidx.compose.runtime.setValue
import androidx.compose.foundation.layout.Column
import org.freedesktop.gstreamer.GStreamer
import java.io.File


class MainActivity : ComponentActivity() {
    init {
        System.loadLibrary("gstreamer_android")
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContent {
            MytestappTheme {
                // A surface container using the 'background' color from the theme
                Surface(
                    modifier = Modifier.fillMaxSize(),
                    color = MaterialTheme.colorScheme.background
                ) {
                    MyAppContent(::onButtonClicked)
                }
            }
        }

        // init gstreamer
        try {
            GStreamer.init(this)
        } catch (e: Exception) {
            e.printStackTrace()
            Log.e("Gstreamer","Couldn't init gstreamer")
            finish()
            return
        }
    }

    fun onButtonClicked() {
        var nativeTest = NativeTest()

        nativeTest.addTrack(
            0,
            File(filesDir, "sample.mp4").absolutePath
        )

        // uncomment to check if nextFrame works
        //nativeTest.nextFrame(0)
    }
}

@Composable
fun MyAppContent(onButtonClicked: () -> Unit) {
    // You can use state to track whether the button is clicked
    var buttonClicked by remember { mutableStateOf(false) }

    Column(
        modifier = Modifier
            .fillMaxSize()
            .padding(16.dp)
    ) {
        if (buttonClicked) {
            onButtonClicked()
            Text("Started. Please see the logs")
        } else {
            StartButton(onClick = { buttonClicked = true })
        }
    }
}

@Composable
fun StartButton(onClick: () -> Unit) {
    Button(onClick = onClick) {
        Text("Start")
    }
}

@Preview(showBackground = true)
@Composable
fun MyAppContentPreview() {
    MytestappTheme {
        MyAppContent(onButtonClicked = {})
    }
}