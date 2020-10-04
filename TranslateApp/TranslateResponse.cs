namespace TranslateApp {
	public class TranslateResponse {
		public class Data {
			public class Translation {
				public string translatedText { get; set; }
			}
			public Translation[] translations { get; set; }
		}
		public Data data { get; set; }
	}
}