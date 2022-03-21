package com.example.fruitappdemo

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import android.widget.Toast
import com.github.mikephil.charting.animation.Easing
import com.github.mikephil.charting.data.Entry
import com.github.mikephil.charting.data.LineData
import com.github.mikephil.charting.data.LineDataSet
import com.google.firebase.database.*
import kotlinx.android.synthetic.main.activity_spectrum.*
import java.lang.Math.pow
import kotlin.math.pow

class SpectrumActivity : AppCompatActivity() {

    lateinit var ref: DatabaseReference
    private var x_value: Float = 0.0f
    private var y_value: Float = 0.0f

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_spectrum)

        ref = FirebaseDatabase.getInstance().getReference("Realtime")
        ref.addValueEventListener(object : ValueEventListener{
            override fun onCancelled(error: DatabaseError) {
                Log.i("Graph Activity", "Data retrieval cancelled")
                Toast.makeText(this@SpectrumActivity, "Data retrieval cancelled", Toast.LENGTH_LONG)
                    .show()
            }

            override fun onDataChange(snapshot: DataSnapshot) {
                Toast.makeText(this@SpectrumActivity, "Data updated", Toast.LENGTH_LONG).show()
                if (snapshot.exists()) {
                    var data = snapshot.getValue(String::class.java)!!

                    // Pixel to Wavelength Conversion
                    val para = arrayListOf<Float>((3.012449224 * (10.0.pow(2.0))).toFloat(), (2.707327106 * (10.0.pow(0.0))).toFloat(), (-1.292606021 * (10.0.pow(-3.0))).toFloat(), (-6.045328702 * (10.0.pow(-6.0))).toFloat(), (2.823900889 * (10.0.pow(-9.0))).toFloat(), (1.380644016 * (10.0.pow(-11.0))).toFloat())

                    val wavelengthArray = arrayListOf<Float>()

                    for (pix in 1..288) {
                        var wavelength: Float = 0f

                        for (i in 0..5) {
                            wavelength += para[i] * (pix.toDouble().pow(i.toDouble())).toFloat()
                        }
                        wavelengthArray.add(wavelength)
                        }

                    // Data Modelling
                    val entries = ArrayList<Entry>()
                    val dataArray = data.split(",").toTypedArray()
                    var x = 0
                    while (x < 288) {
                        x_value = wavelengthArray[x]
                        y_value = dataArray[x].toFloat()
                        entries.add(Entry(x_value, y_value))
                        x++
                    }
                    val vl = LineDataSet(entries, "Spectrum Graph")

                    vl.setDrawValues(false)
                    vl.setDrawFilled(true)
                    vl.lineWidth = 3f
                    vl.fillColor = R.color.colorGray
                    vl.fillAlpha = R.color.colorRed

                    lineChart.xAxis.labelRotationAngle = 0f
                    lineChart.data = LineData(vl)

                    // Modify axis
                    lineChart.axisRight.isEnabled = false
                    lineChart.xAxis.axisMaximum = 880f
                    lineChart.xAxis.axisMinimum = 300f
                    lineChart.setDrawGridBackground(false)
                    lineChart.setGridBackgroundColor(R.color.colorBlack)
                    lineChart.axisLeft.axisMaximum = 1000f
                    lineChart.axisLeft.axisMinimum = 0f

                    // Zoom in - out
                    lineChart.setTouchEnabled(true)
                    lineChart.setPinchZoom(true)
                    lineChart.description.text = "VGU"
                    lineChart.description.setPosition(1900f, 800f)
                    lineChart.description.textSize = 10f
                    lineChart.setNoDataText("No data yet!")

                    lineChart.animateX(1800, Easing.EaseInExpo)

                    val markerView = CustomMarker(this@SpectrumActivity, R.layout.marker_view)
                    lineChart.marker = markerView

                }
                else {
                    Toast.makeText(this@SpectrumActivity, "Data not available", Toast.LENGTH_LONG).show()
                }
            }
        })

    }
}