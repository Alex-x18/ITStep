namespace TranslateApp {
	public class DetectLanguageResponse {
		public class Data {
			public class Detection {
				public double confidence { get; set; }
				public bool isReliable { get; set; }
				public string language { get; set; }
			}
			public Detection[][] detections { get; set; }
		}
		public Data data { get; set; }
	}
}