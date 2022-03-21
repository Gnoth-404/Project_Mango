package com.example.fruitappdemo

import android.content.Context
import com.github.mikephil.charting.components.MarkerView
import com.github.mikephil.charting.data.Entry
import com.github.mikephil.charting.highlight.Highlight
import com.github.mikephil.charting.utils.MPPointF
import kotlinx.android.synthetic.main.marker_view.view.*
import java.math.RoundingMode

class CustomMarker(context: Context, layoutResource: Int):  MarkerView(context, layoutResource) {
    override fun refreshContent(entry: Entry?, highlight: Highlight?) {
        val value_y = entry?.y?.toDouble() ?: 0.0
        val value_y_rounded = value_y.toBigDecimal().setScale(1, RoundingMode.CEILING).toDouble()
        val value_x = entry?.x?.toDouble() ?: 0.0
        val value_x_rounded = value_x.toBigDecimal().setScale(1, RoundingMode.CEILING).toDouble()
        var resText = ""
        if(value_y.toString().length > 8){
            resText = "Value: ${value_x_rounded.toString().substring(0, 7)} ,${
                value_y_rounded.toString().substring(0, 7)
            }"
        }
        else{
            resText = "Value: $value_x_rounded ,$value_y_rounded"
        }
        tvPrice.text = resText
        super.refreshContent(entry, highlight)
    }

    override fun getOffsetForDrawingAtPoint(xpos: Float, ypos: Float): MPPointF {
        return MPPointF(-width / 2f, -height - 10f)
    }
}