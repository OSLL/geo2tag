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
        [StructLayout(LayoutKind.Sequential)]
        struct gps_info
        {
            internal int param1;
            internal int param2;
            internal int param3;
            internal int param4;

            internal short param21;
            internal short param23;
            internal short param22;
            internal short param24;

            internal short param25;
            internal short param26;
            internal short param27;
            internal short param28;

            internal double longitude;
            internal double latitude;
        }

        IntPtr device = IntPtr.Zero;

        IntPtr newLocationHandle = IntPtr.Zero;
        IntPtr deviceStateChangedHandle = IntPtr.Zero;
        IntPtr stopHandle = IntPtr.Zero;

        public Form1()
        {
            InitializeComponent();

            newLocationHandle = CreateEvent(IntPtr.Zero, 0, 0, null);
            deviceStateChangedHandle = CreateEvent(IntPtr.Zero, 0, 0, null);
            stopHandle = CreateEvent(IntPtr.Zero, 0, 0, null);

            device = GPSOpenDevice(newLocationHandle, deviceStateChangedHandle, null, 0);
            if (device != IntPtr.Zero) label1.Text = "Device gotten";

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

            IntPtr position = Marshal.AllocHGlobal(1024*1024);
            GPSGetPosition(device, position, 1000, 0);
            
            try
            {
                if (position != IntPtr.Zero)
                {
                    gps_info coordinates = (gps_info)Marshal.PtrToStructure(position, typeof(gps_info));
                    label1.Text = coordinates.ToString();
                    label1.Text = "Position: " + coordinates.latitude.ToString() + ","
                        + coordinates.longitude.ToString();


                    /*double[] coords = new double[2];
                    
                    Marshal.Copy(position, coords, sizeof(Int32) * (4+18) + sizeof(short) * 8 - 1, 2);
                    label1.Text = coords[0].ToString() + "," + coords[1].ToString();*/

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