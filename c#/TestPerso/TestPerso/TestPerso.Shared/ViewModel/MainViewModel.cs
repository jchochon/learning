using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.IO;
using System.Net.Http;
using System.Reflection;
using System.Windows.Input;
using GalaSoft.MvvmLight;
using GalaSoft.MvvmLight.Command;
using System.Net;
using System.Runtime.InteropServices.ComTypes;
using Windows.Data.Json;
using Windows.Storage.Streams;
using Windows.UI.Xaml.Media.Imaging;
using Newtonsoft.Json;
using TestPerso.Entity;
using System.Threading.Tasks;
using Windows.UI.Core;
using Windows.UI.Xaml.Input;
using Windows.System;

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
        #region private field
        private const string Url = "https://api.betaseries.com";
        private const string SearchShow = "/shows/search";
        private const string GetShowPicture = "/pictures/shows";
        private const string Key = "d99b1417a5b3";
        private readonly HttpClient _httpClient;
        #endregion

        #region backing field
        private TvShow _selectedItem;
        #endregion

        #region property
        public ObservableCollection<TvShow> ResultSearchCollection { get; set; }
        public TvShow SelectedItem 
        {
            get
            {
                return _selectedItem;
            }
            set
            {
                _selectedItem = value;
                RaisePropertyChanged();
            }
        }
        public string SearchInput { get; set; }
        #endregion
        
        #region command
        public ICommand HttpSearchCommand { get; set; }
        public ICommand TestCommand { get; set; }
        #endregion

        
        public MainViewModel()
        {
            HttpSearchCommand = new RelayCommand(HttpSearchExecute);
            TestCommand = new RelayCommand<object>(TestExecute);
            _httpClient = new HttpClient();
            ResultSearchCollection = new ObservableCollection<TvShow>();
        }


        // Test
        public void TestExecute(object e)
        {
            var res = e as KeyRoutedEventArgs;
            if (res.Key == VirtualKey.Enter) ;
        }

        public async void HttpSearchExecute()
        {
            
            ResultSearchCollection.Clear();
            
            // Building query string
            var parameters = new Dictionary<string, string> {{"key", Key}, {"title", SearchInput}};
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

                var tmp = Url + GetShowPicture + queryString;
                Stream resp = new MemoryStream();
                try
                {
                    resp = await _httpClient.GetStreamAsync(Url + GetShowPicture + queryString);
                }
                catch (HttpRequestException ex)
                {
                    // If HttpRequestException match to error 404 not found for a picture
                    if (ex.HResult == -2146233088)
                    {
                        ResultSearchCollection.Add(new TvShow()
                        {
                            Show = show
                        });
                        continue;
                    }
                    else
                        throw;
                }
                // Converting Win32 stream type to WinRT stream type?
                MemoryStream memoryStream = new MemoryStream();
                await resp.CopyToAsync(memoryStream);

                var randomAccessStream = new InMemoryRandomAccessStream();
                var output = randomAccessStream.GetOutputStreamAt(0);
                var dataWriter = new DataWriter(output);
                //var task = Task.Factory.StartNew(() => dataWriter.WriteBytes(memoryStream.ToArray()));
                dataWriter.WriteBytes(memoryStream.ToArray());
                await dataWriter.StoreAsync();

                var image = new BitmapImage();
                await image.SetSourceAsync(randomAccessStream);
                ResultSearchCollection.Add(new TvShow()
                {
                    Image = image,
                    Show = show
                });
            }
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