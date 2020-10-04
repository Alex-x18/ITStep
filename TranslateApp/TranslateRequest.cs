namespace TranslateApp {
	public class TranslateRequest {
		public TranslateRequest(string q, string source, string target, string format) {
			this.q = q;
			this.source = source;
			this.target = target;
			this.format = format;
		}
		public string q { get; set; }
		#pragma warning restore IDE1006
		public string source { get; set; }
		public string target { get; set; }
		public string format { get; set; }
	}
}