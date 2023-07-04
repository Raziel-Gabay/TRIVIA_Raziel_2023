using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using System.Text.Json;
using System.Collections;
using System.IO;
using System.Xml.Linq;
using Newtonsoft.Json.Linq;
using Newtonsoft.Json;
using System.Text.Json.Nodes;

namespace Trivia_Project_By_R.G
{
    /// <summary>
    /// Interaction logic for LoginWindow.xaml
    /// </summary>
    public partial class LoginWindow : Window
    {
        public const uint LOGIN_CODE = 100;
 
        public LoginWindow()
        {

            InitializeComponent();
            try
            {
                // Create a TcpClient instance and connect to localhost on a specific port
                m_client = new TcpClient("localhost", 8826);

                // Check if the client is connected
                if (m_client.Connected)
                {
                    Console.WriteLine("Connected to localhost!");

                    // Perform operations on the TcpClient here

                    // Close the TcpClient connection
                    Console.WriteLine("Disconnected from localhost.");
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine("Error: " + ex.Message);
            }
        }
        public LoginWindow(TcpClient client)
        {
            InitializeComponent();
            m_client = client;
        }

        private void OpenWindow(object sender, RoutedEventArgs e)
        {
            var jsonObject = new JObject
            {
                { "username", username.Text},
                { "password",password.Text },
            };

            byte[] data = serializeMessage(jsonObject, LOGIN_CODE);

            NetworkStream stream = this.m_client.GetStream();
            stream.Write(data, 0, data.Length);

            byte[] buffer = new byte[1024];
            int bytesRead = stream.Read(buffer, 0, buffer.Length);
            string response = Encoding.UTF8.GetString(buffer, 0, bytesRead);

            string serverMSG = response.Substring(5);
            JObject joRecive = JObject.Parse(serverMSG);

            if (joRecive.ContainsKey("status"))
            {
                MainWindow objMainWindow = new MainWindow(m_client, username.Text.ToString());
                this.Visibility = Visibility.Hidden;
                objMainWindow.Show();
            }
            else
            {
                MessageBox.Show("You entered an incorrect username or password!");
            }
        }

        private void OpenSignup(object sender, RoutedEventArgs e)
        {
            SignupWindow objSignupWindow = new SignupWindow(m_client);
            this.Visibility = Visibility.Hidden;
            objSignupWindow.Show();
        }

        private void Exit(object sender, RoutedEventArgs e)
        {
            System.Windows.Application.Current.Shutdown();
        }

        static public byte[] serializeMessage(JObject messageJson, uint code)
        {
            string jsonString = JsonConvert.SerializeObject(messageJson);

            int byteSize = Encoding.UTF8.GetByteCount(jsonString);
            BitArray bitArray = new BitArray(BitConverter.GetBytes(byteSize));

            string clientMSG = (char)code + BitArrayToFourChars(bitArray) + jsonString;

            return Encoding.UTF8.GetBytes(clientMSG);
        }
        static string BitArrayToFourChars(BitArray bitArray)
        {
            string result = string.Empty;

            for (int i = 0; i < bitArray.Length; i += 8)
            {
                int decimalValue = 0;
                for (int j = 0; j < 8; j++)
                {
                    if (i + j < bitArray.Length && bitArray[i + j])
                    {
                        decimalValue += 1 << (7 - j);
                    }
                }
                result += (char)decimalValue;
            }
            result = new string(result.Reverse().ToArray());
            return result;
        }
        static public int FourCharsToInt(string fourChars)
        {
            if (fourChars.Length != 4)
            {
                throw new ArgumentException("Input string must have exactly four characters.");
            }

            int intValue = 0;
            for (int i = 0; i < 4; i++)
            {
                intValue = (intValue << 8) | (byte)fourChars[i];
            }

            return intValue;
        }


        private TcpClient m_client;

    }
}
class JsonObject : System.Collections.Generic.Dictionary<string, object> { }
