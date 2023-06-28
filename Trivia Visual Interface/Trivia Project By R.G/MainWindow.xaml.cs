using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Net.Sockets;
using Newtonsoft.Json.Linq;
using System.Windows.Markup;

namespace Trivia_Project_By_R.G
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public const uint LOGOUT_CODE = 102;
        public const uint GET_ROOMS_CODE = 103;

        public MainWindow(TcpClient client, string username)
        {
            InitializeComponent();
            m_client = client;
            this.m_username = username;
        }
        private TcpClient m_client;
        private string m_username;

        private void OpenCreateRoomWindow(object sender, RoutedEventArgs e)
        {
            CreateRoomWindow objCreateRoomWindow = new CreateRoomWindow(m_client, m_username);
            this.Visibility = Visibility.Hidden;
            objCreateRoomWindow.Show();
        }

        private void OpenJoinRoomWindow(object sender, RoutedEventArgs e)
        {
            JoinRoomWindow objJoinRoomWindow = new JoinRoomWindow(m_client, m_username);
            JObject emptyJson = new JObject();
            byte[] data = LoginWindow.serializeMessage(emptyJson, GET_ROOMS_CODE);

            NetworkStream stream = this.m_client.GetStream();
            stream.Write(data, 0, data.Length);

            byte[] buffer = new byte[1024];
            int bytesRead = stream.Read(buffer, 0, buffer.Length);
            string response = Encoding.UTF8.GetString(buffer, 0, bytesRead);

            string serverMSG = response.Substring(5);
            
            JObject joRecive = JObject.Parse(serverMSG);
            if (joRecive.ContainsKey("status"))
            {
                //JArray joArrRooms = (JArray)joRecive["Rooms"];

                foreach (var element in joRecive["Rooms"])
                {
                    if (element["isActive"].Value<bool>())
                    {
                        objJoinRoomWindow.AddItem(element["name"].ToString());
                    }
                   
                }

                this.Visibility = Visibility.Hidden;
                objJoinRoomWindow.Show();
            }
            else
            {
            }

           
        }

        private void OpenStatisticsWindow(object sender, RoutedEventArgs e)
        {
            StatisticsWindow objStatisticsWindow = new StatisticsWindow(m_client, m_username);
            this.Visibility = Visibility.Hidden;
            objStatisticsWindow.Show();
        }

        private void Exit(object sender, RoutedEventArgs e)
        {

            JObject emptyJson = new JObject();
            byte[] data = LoginWindow.serializeMessage(emptyJson, LOGOUT_CODE);

            NetworkStream stream = this.m_client.GetStream();
            stream.Write(data, 0, data.Length);

            byte[] buffer = new byte[1024];
            int bytesRead = stream.Read(buffer, 0, buffer.Length);
            string response = Encoding.UTF8.GetString(buffer, 0, bytesRead);

            string serverMSG = response.Substring(5);
            JObject joRecive = JObject.Parse(serverMSG);

            if (joRecive.ContainsKey("status"))
            {
                m_client.Close();
                System.Windows.Application.Current.Shutdown();
            }
            else
            {
            }
        }

        private void Logout(object sender, RoutedEventArgs e)
        {
            JObject emptyJson = new JObject();
            byte[] data = LoginWindow.serializeMessage(emptyJson, LOGOUT_CODE);

            NetworkStream stream = this.m_client.GetStream();
            stream.Write(data, 0, data.Length);

            byte[] buffer = new byte[1024];
            int bytesRead = stream.Read(buffer, 0, buffer.Length);
            string response = Encoding.UTF8.GetString(buffer, 0, bytesRead);

            string serverMSG = response.Substring(5);
            JObject joRecive = JObject.Parse(serverMSG);

            if (joRecive.ContainsKey("status"))
            {
                LoginWindow objLoginWindow = new LoginWindow(m_client);
                this.Visibility = Visibility.Hidden;
                objLoginWindow.Show();
            }
            else
            {
            }
        }
    }
}
