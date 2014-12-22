using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Net.Http;
using System.Reflection;
using System.Windows.Input;
using GalaSoft.MvvmLight;
using GalaSoft.MvvmLight.Command;
using System.Net;
using System.Runtime.InteropServices.ComTypes;
using Windows.Data.Json;
using Windows.UI.Xaml.Media.Imaging;
using Newtonsoft.Json;
using TestPerso.Entity;

namespace TestPerso.ViewModel
{
    /// <summary>
    /// This class contains properties that the main View can data bind to.
    /// <para>
    /// Use the <strong>mvvminpc</strong> snippet to add bindable properties to this ViewModel.
    /// </para>
    /// <para>
    /// You can also use Blend to data bind with the tool's support.
    /// </para>
    /// <para>
    /// See http://www.galasoft.ch/mvvm
    /// </para>
    /// </summary>
    public class MainViewModel : ViewModelBase
    {
        private const string Url = "https://api.betaseries.com";
        private const string SearchShow = "/shows/search";
        private const string GetShowPicture = "pictures/shows";
        private const string Key = "d99b1417a5b3";
        public ObservableCollection<List<object>> ResultSearchCollection { get; set; }

        public string SearchInput { get; set; }

        public ICommand SendHttpRequestCommand { get; set; }

        private HttpClient _httpClient;

        public MainViewModel()
        {
            SendHttpRequestCommand = new RelayCommand(SendHttpRequestExecute);
            _httpClient = new HttpClient();

            ResultSearchCollection = new ObservableCollection<List<object>>();
            
        }


        public async void SendHttpRequestExecute()
        {
            
            ResultSearchCollection.Clear();
            
            // Building query string
            var parameters = new Dictionary<string, string>();
            parameters.Add("key", Key);
            parameters.Add("title", SearchInput);
            var queryString = QueryStringBuilder(parameters);
            
            // Sending get request
            HttpResponseMessage response = await _httpClient.GetAsync(Url + SearchShow + queryString);
            //response.EnsureSuccessStatusCode();

            //ResultSearchCollection.Add(response.StatusCode.ToString());
            
            // Waiting http response
            var content = await response.Content.ReadAsStringAsync();

            var res = JsonConvert.DeserializeObject<RootObject>(content);

            foreach (var show in res.shows)
            {
                parameters.Clear();
                parameters.Add("key", Key);
                parameters.Add("id", show.id.ToString());
                queryString = QueryStringBuilder(parameters);
                ResultSearchCollection.Add(new List<object>
                {
                    new BitmapImage(new Uri(Url + GetShowPicture + queryString)),
                    show
                });
            }
            
            //foreach (Show show in res.shows)
            //{
            //    ResultSearchCollection.Add(show.title);
            //    ResultSearchCollection.Add("__________________");
            //    ResultSearchCollection.Add(show.description);
            //    ResultSearchCollection.Add(show.
            //        ResultSearchCollection.Add("__________________________________________________________________");
            //}

        }

        private string QueryStringBuilder(Dictionary<string, string> queryStringDictionary)
        {
            string queryString = "";

            var boolean = true;
            foreach (KeyValuePair<string, string> kvp in queryStringDictionary)
            {
                if (!boolean)
                    queryString += "&" + kvp.Key + "=" + kvp.Value;
                else
                {
                    queryString += "?" + kvp.Key + "=" + kvp.Value;
                    boolean = false;
                }
            }

            return queryString;
        }
    }
}