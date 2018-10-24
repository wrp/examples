
import java.util.HashMap;
import java.util.Map;



public class map_example {
	static class cmdlinearg {
		public String short_name;
		public String description;
		public Boolean required;

		cmdlinearg(String short_name, String description, Boolean req) {
			this.short_name = short_name;
			this.description = description;
			this.required = req;
		}
	}

	public static void main(String args[]) {
		Map<Integer, String> m = new HashMap<Integer, String>();
		m.put(5, "five");
		for(Map.Entry mp:m.entrySet()) {  
			System.out.println(mp.getKey() + ": "  + mp.getValue());  
		}  
		System.out.println("direct access: " + m.get(5));

		Map<String, cmdlinearg> v = new HashMap<String, cmdlinearg>();
		v.put("test", new cmdlinearg("t", "test func", true));
		for(Map.Entry mp:v.entrySet()) {  
			System.out.println(mp.getKey() + ": "  + mp.getValue());  
		}  
		System.out.println("direct access: " + v.get("test").description);
	}
}
