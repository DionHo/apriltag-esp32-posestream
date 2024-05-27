extends Spatial

export var tcp_address = "192.168.1.200"
export var tcp_port = 8080

var tcp_peer = StreamPeerTCP.new()

func _ready():
	tcp_peer.connect_to_host(tcp_address,tcp_port)
	
func _process(delta):
	match tcp_peer.get_status():
		tcp_peer.STATUS_CONNECTING:
			pass
		tcp_peer.STATUS_CONNECTED:
			while tcp_peer.get_available_bytes():
				var data_len = tcp_peer.get_available_bytes()
				var data = PoolByteArray(tcp_peer.get_data(data_len)[1])
				print("Got new data: '%s'" % data.get_string_from_ascii())
				$RichTextLabel.text += data.get_string_from_ascii() + "\n"
				var result = JSON.parse(data.get_string_from_ascii())
				if len(result.result) > 0:
					var d = result.result[0]
					print(d)
					$AprilTag.transform.basis.x = Vector3(d["rot"][0],d["rot"][3],d["rot"][6])
					$AprilTag.transform.basis.y = Vector3(d["rot"][1],d["rot"][4],d["rot"][7])
					$AprilTag.transform.basis.z = Vector3(d["rot"][2],d["rot"][5],d["rot"][8])
					$AprilTag.transform.origin  = Vector3(d["t"][0],  d["t"][1],  d["t"][2])
		tcp_peer.STATUS_ERROR:
			tcp_peer.connect_to_host(tcp_address,tcp_port)
		tcp_peer.STATUS_NONE:
			tcp_peer.connect_to_host(tcp_address,tcp_port)

