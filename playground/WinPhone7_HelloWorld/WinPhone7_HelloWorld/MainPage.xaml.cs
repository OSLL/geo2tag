using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.IO;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Shapes;
using System.Device.Location;

using Microsoft.Phone.Controls;

namespace WinPhone7_HelloWorld
{
    public partial class MainPage : PhoneApplicationPage
    {
        GeoCoordinateWatcher watcher;
        // Constructor
        public MainPage()
        {
            InitializeComponent();
            // Init watcher with default accuracy
            watcher = new GeoCoordinateWatcher();
            // Start listen for coordinates
            watcher.TryStart(false, TimeSpan.FromMilliseconds(1000));
        }

        private void button1_Click(object sender, RoutedEventArgs e)
        {
            MessageBox.Show("HelloWorld");
        }

        //Here will be GET request
        private void button2_Click(object sender, RoutedEventArgs e)
        {
            textBlock1.Text = "Doing GET request";
            WebClient wc = new WebClient();
            string sRequest = "http://tracklife.ru";
            // Adding onDownloadComplete event handler
            wc.DownloadStringCompleted += new DownloadStringCompletedEventHandler(wc_DownloadStringCompleted);
            // Download
            wc.DownloadStringAsync(new Uri(sRequest));
        }

        //GET request event handler
        private void wc_DownloadStringCompleted(object sender, DownloadStringCompletedEventArgs e)
        {
            textBlock1.Text = e.Result;
        }

        //Here will be POST request
        private void button3_Click(object sender, RoutedEventArgs e)
        {
            textBlock1.Text = "Doing POST request";
            String url = "http://tracklife.ru/service?query=login";
            HttpWebRequest request = (HttpWebRequest)WebRequest.Create(new Uri(url, UriKind.Absolute)); 
            request.Method = "POST"; 
            // don't miss out this 
            request.ContentType = "application/x-www-form-urlencoded"; 
            // Adding function that will fill POST data
            request.BeginGetRequestStream(new AsyncCallback(RequestReady), request); 


        }

        void RequestReady(IAsyncResult asyncResult)
        {
            // Casting argument to HttpWebRequest           
            HttpWebRequest request = asyncResult.AsyncState as HttpWebRequest;
            Stream stream = request.EndGetRequestStream(asyncResult);

            // Hack for solving multi-threading problem 
            // I think this is a bug 
            this.Dispatcher.BeginInvoke(delegate()
            {
                // Send the post variables 
                StreamWriter writer = new StreamWriter(stream);
                String postData = "{\"login\":\"Vasja\",\"password\":\"VasjaPWD\"}";
                writer.Write(postData);
                writer.Flush();
                writer.Close();
                // Setting up event handler for onResponseRecieved event
                request.BeginGetResponse(new AsyncCallback(ResponseReady), request);
            });
        }
        
        void ResponseReady(IAsyncResult asyncResult)
        {
            // Casting argument to HttpWebRequest
            HttpWebRequest request = asyncResult.AsyncState as HttpWebRequest;
            HttpWebResponse response = (HttpWebResponse)request.EndGetResponse(asyncResult);
            this.Dispatcher.BeginInvoke(delegate()
            {
                Stream responseStream = response.GetResponseStream();
                StreamReader reader = new StreamReader(responseStream);
                // get the result text 
                string result = reader.ReadToEnd();
                textBlock1.Text = result;
                // Send to the parser! 
                // TODO parse the result

            });
        }

        private void button4_Click(object sender, RoutedEventArgs e)
        {
            textBlock1.Text = "Getting location...";

            GeoCoordinate coord = watcher.Position.Location;
            // Check validity of coordinates
            if (coord.IsUnknown != true)
            {
                textBlock1.Text = "Lat.:" + coord.Latitude.ToString() + ", lon.:" + coord.Longitude.ToString();
            }
            else
            {
                textBlock1.Text = "Error getting coordinates";
            }

        }
    }
}