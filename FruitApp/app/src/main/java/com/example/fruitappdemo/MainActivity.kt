package com.example.fruitappdemo

import android.content.BroadcastReceiver
import android.content.Context
import android.content.Intent
import android.content.IntentFilter
import android.graphics.PorterDuff
import android.net.wifi.ScanResult
import android.net.wifi.WifiConfiguration
import android.net.wifi.WifiEnterpriseConfig
import android.net.wifi.WifiManager
import android.os.Bundle
import android.util.Log
import android.view.MotionEvent
import android.view.View
import android.view.ViewParent
import android.widget.*
import androidx.appcompat.app.AppCompatActivity
import kotlinx.android.synthetic.main.activity_main.*

@Suppress("DEPRECATION")
class MainActivity : AppCompatActivity(), AdapterView.OnItemClickListener {

    lateinit var wifiManager: WifiManager
    lateinit var results: List<ScanResult>
    lateinit var adapter1: ArrayAdapter<String>
    lateinit var arrayList: ArrayList<String>
    lateinit var listView: ListView

    private var wifiReceiver = object: BroadcastReceiver() {
        override fun onReceive(context: Context, intent: Intent) {
            results = wifiManager.scanResults
            unregisterReceiver(this)

            for (result in results) {
                arrayList.add(result.SSID)
                adapter1.notifyDataSetChanged()
            }
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        // Variables
        val spectrum_mode = findViewById<Button>(R.id.Spectrum)
        val wavelength_mode = findViewById<Button>(R.id.Wavelength)
        val scan_wifi = findViewById<Button>(R.id.scanwifi)
        listView = findViewById(R.id.list_wifi)
        wifiManager = applicationContext.getSystemService(Context.WIFI_SERVICE) as WifiManager
        arrayList = arrayListOf("WiFi Scanner")

        if (!wifiManager.isWifiEnabled) {
            Toast.makeText(this, "Wifi is disabled ... you need to enable it", Toast.LENGTH_LONG).show()
            wifiManager.isWifiEnabled = true
        }

        adapter1 = ArrayAdapter(this, android.R.layout.simple_list_item_1, arrayList)
        listView.adapter = adapter1
        scanwifi()

        listView.onItemClickListener = this

        fun buttonEffect(button: View) {
            button.setOnTouchListener(fun(v: View, event: MotionEvent): Boolean {
                when (event.action) {
                    MotionEvent.ACTION_DOWN -> {
                        v.background.setColorFilter(-0x77000000, PorterDuff.Mode.SRC_ATOP)
                        v.invalidate()
                    }
                    MotionEvent.ACTION_UP -> {
                        v.background.clearColorFilter()
                        v.invalidate()
                    }
                }
                return false
            })
        }

        buttonEffect(scan_wifi)
        buttonEffect(spectrum_mode)
        buttonEffect(wavelength_mode)

        spectrum_mode.setOnClickListener {
            Log.d("Main Activity", "Switch to spectrum mode")
            val intent = Intent(this, SpectrumActivity::class.java)
            startActivity(intent)
        }

        wavelength_mode.setOnClickListener {
            Log.d("Main Activity", "Switch to wavelength mode")
            val intent = Intent(this, WavelengthActivity::class.java)
            startActivity(intent)
        }

        scan_wifi.setOnClickListener {
            Log.d("Main Activity", "Scanning Wifi")
            scanwifi()
        }
    }

    private fun scanwifi() {
        arrayList.clear()
        registerReceiver(wifiReceiver, IntentFilter(WifiManager.SCAN_RESULTS_AVAILABLE_ACTION))
        wifiManager.startScan()
        Toast.makeText(this, "Scanning Wifi", Toast.LENGTH_SHORT).show()
    }

    override fun onItemClick(parent: AdapterView<*>?, view: View?, position: Int, id: Long) {
        val wificonfig =  WifiConfiguration()
        val wifi = parent?.getItemAtPosition(position).toString()
        val pass = "123456789"
        Toast.makeText(this, "Connecting to $wifi", Toast.LENGTH_SHORT).show()

        wificonfig.SSID = "\"" + wifi + "\""
        wificonfig.preSharedKey = "\"" + pass + "\""

        wifiManager.disconnect()
        wifiManager.enableNetwork(wifiManager.addNetwork(wificonfig), true)
        wifiManager.reconnect()
    }

}

