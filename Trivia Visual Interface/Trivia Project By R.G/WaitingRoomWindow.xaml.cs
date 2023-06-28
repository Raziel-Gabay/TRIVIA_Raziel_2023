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
    /// Interaction logic for WaitingRoomWindow.xaml
    /// </summary>
    public partial class WaitingRoomWindow : Window
    {
        public WaitingRoomWindow(TcpClient client, string username)
        {
            InitializeComponent();
            m_client = client;
            this.m_username = username;
        }
        private TcpClient m_client;
        private string m_username;

        private void BackToLastWindow(object sender, RoutedEventArgs e)
        {
            if (m_username == Admin.Content.ToString())
            {
                CreateRoomWindow objCreateRoomWindow = new CreateRoomWindow(m_client, m_username);
                this.Visibility = Visibility.Hidden;
                objCreateRoomWindow.Show();
            }
        }
        public void AddItem(string roomName)
        {
            if (!string.IsNullOrEmpty(roomName))
            {
                playersListBox.Items.Add(roomName);
            }
        }
    }
}
