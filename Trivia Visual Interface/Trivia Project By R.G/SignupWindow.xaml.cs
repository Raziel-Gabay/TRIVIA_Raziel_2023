using Newtonsoft.Json.Linq;
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

namespace Trivia_Project_By_R.G
{
    /// <summary>
    /// Interaction logic for SignupWindow.xaml
    /// </summary>
    public partial class SignupWindow : Window
    {
        public const uint SIGNUP_CODE = 101;

        public SignupWindow(TcpClient client)
        {
            InitializeComponent();
            m_client = client;
        }


        private void AbortToLoginWindow(object sender, RoutedEventArgs e)
        {
            LoginWindow objLoginWindow = new LoginWindow(m_client);
            this.Visibility = Visibility.Hidden;
            objLoginWindow.Show();
        }
        private TcpClient m_client;

        private void Signup(object sender, RoutedEventArgs e)
        {
            var jsonObject = new JObject
            {
                { "username", username.Text},
                { "password",password.Text }
            };
            if (string.IsNullOrEmpty(email.Text))
            {
                jsonObject.Add("mail", "null");
            }
            else
            {
                jsonObject.Add("mail", email.Text);
            }

            byte[] data = LoginWindow.serializeMessage(jsonObject, SIGNUP_CODE);

            NetworkStream stream = this.m_client.GetStream();
            stream.Write(data, 0, data.Length);

            byte[] buffer = new byte[1024];
            int bytesRead = stream.Read(buffer, 0, buffer.Length);
            string response = Encoding.UTF8.GetString(buffer, 0, bytesRead);

            string serverMSG = response.Substring(5);
            JObject joRecive = JObject.Parse(serverMSG);

            if (joRecive.ContainsKey("status"))
            {
                AbortToLoginWindow(sender, e);
            }
            else
            {
                MessageBox.Show("The username already exists in the system!");
            }
        }
    }
}
