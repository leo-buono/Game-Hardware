using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Respawn : MonoBehaviour
{
	public Vector3 spawnPos = Vector3.up * 2f;

	private void OnTriggerEnter(Collider other) {
		other.transform.position = spawnPos;
	}
}
