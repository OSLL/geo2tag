using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Net;
using System.IO;
using System.Runtime.InteropServices;
using System.Collections;

namespace HelloWorld
{
    public partial class Form1 : Form
    {

        IntPtr device = IntPtr.Zero;


        public Form1()
        {
            InitializeComponent();


            device = GPSOpenDevice(IntPtr.Zero, IntPtr.Zero, null, 0);
            if (device != IntPtr.Zero) label1.Text = "Device gotten";
            else
            {
                label1.Text = "Error while opening device";
            }

        }




        ~Form1()
        {
            GPSCloseDevice(device);
        }



        private void button1_Click(object sender, EventArgs e)
        {
            MessageBox.Show("HelloWorld");
        }

        // This method will do POST request
        private void button2_Click(object sender, EventArgs e)
        {
            
            
            label1.Text = "Sending POST request ...";
            try
            {
                String postData = "{\"login\":\"Vasja\",\"password\":\"VasjaPWD\"}";
                String url = "http://tracklife.ru/service?query=login";
                label1.Text = "Creating HttpWebRequest";
                HttpWebRequest req = (HttpWebRequest)WebRequest.Create(url);
                req.Method = "POST";
                req.KeepAlive = false;
                req.AllowWriteStreamBuffering = true;
                req.ContentType = "application/x-www-form-urlencoded";
                
                req.ContentLength = postData.Length;
                req.Timeout = 20000;
                label1.Text = "Getting request stream";
                Stream requestStream = req.GetRequestStream();
                label1.Text = "Writing POST data to request stream";
                requestStream.Write(Encoding.ASCII.GetBytes(postData), 0,postData.Length);
                requestStream.Close();

                label1.Text = "closing request stream";
                WebResponse resp = req.GetResponse();
                Stream responseStream = resp.GetResponseStream();
                StreamReader reader = new StreamReader(responseStream);
                label1.Text = reader.ReadToEnd();
                responseStream.Close();
                reader.Close();
            }
            catch (WebException ex)
            {
               label1.Text += ex.Message;
            }
        }
        // This method will do GET request
        private void button3_Click(object sender, EventArgs e)
        {
            label1.Text = "Sending GET request ...";
            try
            {
                HttpWebRequest req = (HttpWebRequest)WebRequest.Create("http://tracklife.ru");
                req.Method = "GET";
                WebResponse resp = req.GetResponse();
                // Read response stream
                Stream responseStream = resp.GetResponseStream();
                StreamReader reader = new StreamReader(responseStream);
                // Convert response stream to string
                label1.Text = reader.ReadToEnd();
            }
            catch (WebException ex)
            {
                label1.Text = ex.Message;
            }
        }
        // This will print current location
        private void button4_Click(object sender, EventArgs e)
        {
            //Here will be code for location 

            label1.Text = "Locating.....";
            // Prepare GpsPosition struct 
            GpsPosition coordinates = new GpsPosition();
            coordinates.dwVersion = 1;
            coordinates.dwSize = Marshal.SizeOf(typeof(GpsPosition));

            
            IntPtr position = Marshal.AllocHGlobal(Marshal.SizeOf(typeof(GpsPosition)));
            // Marshal our data to the native pointer we allocated.
            Marshal.StructureToPtr(coordinates, position, false);
            GPSGetPosition(device, position, 10000, 0);
            
            try
            {
                if (position != IntPtr.Zero)
                {

                    coordinates = (GpsPosition)Marshal.PtrToStructure(position, typeof(GpsPosition));
                 
                    label1.Text = "Position: " + coordinates.Latitude + ","
                        + coordinates.Longitude;

                }
                else 
                {
                    label1.Text = "Position is null!";
                }
            }
            catch (Exception ex)
            {
                label1.Text = ex.Message;
            }
            //
            Marshal.FreeHGlobal(position);
        }

        #region PInvokes to gpsapi.dll
        [DllImport("gpsapi.dll")]
        static extern IntPtr GPSOpenDevice(IntPtr hNewLocationData, IntPtr hDeviceStateChange, string szDeviceName, int dwFlags);

        [DllImport("gpsapi.dll")]
        static extern int GPSCloseDevice(IntPtr hGPSDevice);

        [DllImport("gpsapi.dll")]
        static extern int GPSGetPosition(IntPtr hGPSDevice, IntPtr pGPSPosition, int dwMaximumAge, int dwFlags);

        [DllImport("gpsapi.dll")]
        static extern int GPSGetDeviceState(IntPtr pGPSDevice);
        #endregion


        #region PInvokes to coredll.dll
        [DllImport("coredll.dll")]
        static extern IntPtr CreateEvent(IntPtr lpEventAttributes, int bManualReset, int bInitialState, StringBuilder lpName);

        [DllImport("coredll.dll")]
        static extern int CloseHandle(IntPtr hObject);

        const int waitFailed = -1;
        [DllImport("coredll.dll")]
        static extern int WaitForMultipleObjects(int nCount, IntPtr lpHandles, int fWaitAll, int dwMilliseconds);

        const int eventSet = 3;
        [DllImport("coredll.dll")]
        static extern int EventModify(IntPtr hHandle, int dwFunc);

        #endregion
    }
}