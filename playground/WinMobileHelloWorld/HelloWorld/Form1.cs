using System;

using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Net;
using System.IO;

namespace HelloWorld
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
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
    }
}