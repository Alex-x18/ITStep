using System;
using System.Collections.Generic;
using System.Text.Json;
using System.Net;

namespace WeatherApp {
	public class GeoInfo {
		public GeoInfo(string geoApiKey) {
			ApiKey = geoApiKey;
			Response = null;
			if (ApiKey == null) {
				throw new ArgumentNullException("Malformed object: an argument cannot be null");
			}
		}
		public void Update() {
			using var client = new WebClient();
			var ip = client.DownloadString("http://icanhazip.com");
			var response = client.DownloadString($@"http://api.ipstack.com/{ip}?access_key={ApiKey}");
			Response = JsonSerializer.Deserialize<GeopositionResponse>(response);
		}
		public decimal? Lon {
			get {
				return Response?.longitude;
			}
		}
		public decimal? Lat {
			get {
				return Response?.latitude;
			}
		}
		public string City {
			get {
				return Response?.city;
			}
		}
		public string Country {
			get {
				return Response?.country_name;
			}
		}
		public string Isp {
			get {
				return Response?.connection?.isp;
			}
		}
		public List<string> Languages {
			get {
				var list = new List<string>();
				foreach (var item in Response?.location?.languages) {
					list.Add(item.name);
				}
				return list;
			}
		}
		public string Hostname {
			get {
				return Response?.hostname;
			}
		}
		public string Ip {
			get {
				return Response?.ip;
			}
		}
		private readonly string ApiKey;
		private GeopositionResponse Response { get; set; }
	}
}
